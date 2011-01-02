#include <vtkObjectFactory.h> //for new() macro
#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkTree.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPolyData.h>
#include <vtkMath.h>
#include <vtkKdTreePointLocator.h>
#include <vtkIntArray.h>
#include <vtkImageData.h>
#include <vtkPlane.h>
#include <vtkDiscreteMarchingCubes.h>
#include <vtkContourFilter.h>

#include <vtkXMLImageDataWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLPolyDataWriter.h>

#include "vtkPointSetSurfaceReconstruction.h"
#include "vtkVoxelizePolyData.h"

vtkStandardNewMacro(vtkPointSetSurfaceReconstruction);

vtkPointSetSurfaceReconstruction::vtkPointSetSurfaceReconstruction()
{
  this->SamplesPerDimension = 10;
  this->Border = 5;
}

int vtkPointSetSurfaceReconstruction::RequestData(vtkInformation *vtkNotUsed(request),
                                             vtkInformationVector **inputVector,
                                             vtkInformationVector *outputVector)
{
  //This function calls the scanners input and output to allow it to
  //work in the vtk algorithm pipeline

  // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Create a KDTree of the input points
  vtkSmartPointer<vtkKdTreePointLocator> pointTree =
    vtkSmartPointer<vtkKdTreePointLocator>::New();
  pointTree->SetDataSet(input);
  pointTree->BuildLocator();

  // Get normals from polydata
  vtkSmartPointer<vtkDoubleArray> normals =
    vtkDoubleArray::SafeDownCast(input->GetPointData()->GetNormals());

  if(!normals)
    {
    vtkErrorMacro("The input to vtkPointSetSurfaceReconstruction must have point normals.");
    return 0;
    }

  vtkSmartPointer<vtkVoxelizePolyData> voxelizeFilter =
    vtkSmartPointer<vtkVoxelizePolyData>::New();
  voxelizeFilter->SetInput(input);
  voxelizeFilter->SetNumberOfCellsAll(this->SamplesPerDimension);
  voxelizeFilter->SetBorder(this->Border);
  voxelizeFilter->Update();

  vtkImageData* grid = voxelizeFilter->GetOutput();

/*
  {
  vtkSmartPointer<vtkXMLImageDataWriter> writer =
      vtkSmartPointer<vtkXMLImageDataWriter>::New();
  writer->SetFileName("grid.vti");
  writer->SetInput(grid);
  writer->Write();
  }
*/

  grid->SetNumberOfScalarComponents(1);

  int* dims = grid->GetDimensions();

  for (int z = 0; z < dims[2] - 1; z++)
    {
    for (int y = 0; y < dims[1] - 1; y++)
      {
      for (int x = 0; x < dims[0] - 1; x++)
        {
        //get cell center
        double center[3];
        GetCellCenter(grid, x, y, z, center);

        //find closest point to this cell
        vtkIdType ID = pointTree->FindClosestPoint(center);
        double closestPoint[3];
        input->GetPoint(ID, closestPoint);
        //compute the distance from the cell center to the plane defined by the normal of the closest point
        //get the normal of the closest point
        double n[3];
        normals->GetTuple(ID, n);

        // Find the distance from the cell center to the plane defined by this point and normal
        //double distance = vtkPlane::DistanceToPlane(center, n, closestPoint);
        //vtkstd::cout << "distance: " << distance << vtkstd::endl;
        double signedDistance = SignedDistanceToPlane(center, n, closestPoint);
        //vtkstd::cout << "Signed distance: " << signedDistance << vtkstd::endl;

        grid->SetScalarComponentFromDouble(x,y,z,0,signedDistance);
        }
      }
    }

  /*
  {
  vtkSmartPointer<vtkXMLImageDataWriter> writer =
      vtkSmartPointer<vtkXMLImageDataWriter>::New();
  writer->SetFileName("grid_values.vti");
  writer->SetInput(grid);
  writer->Write();
  }
  */

  vtkSmartPointer<vtkContourFilter> contourFilter =
    vtkSmartPointer<vtkContourFilter>::New();
  contourFilter->SetValue(0, 0);
  contourFilter->SetInput(grid);
  contourFilter->Update();

  vtkPolyData* surface = contourFilter->GetOutput();

  output->ShallowCopy(contourFilter->GetOutput());

  return 1;
}

////////// External Operators /////////////

void vtkPointSetSurfaceReconstruction::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}


////////////////////
//Helper functions//
////////////////////
double SignedDistanceToPlane(double x[3], double n[3], double p0[3])
{
  double d = vtkPlane::DistanceToPlane(x,n,p0);
  double v[3];
  vtkMath::Subtract(x, p0, v);

  if(vtkMath::Dot(v, n) > 0.0)
    {
    return d;
    }
  else
    {
    return -d;
    }
}

void GetCellCenter(vtkImageData* image, unsigned int i, unsigned int j, unsigned int k, double center[3])
{
  double origin[3];
  image->GetOrigin(origin);

  double spacing[3];
  image->GetSpacing(spacing);

  center[0] = origin[0] + i * spacing[0] + spacing[0]/2.0;
  center[1] = origin[1] + j * spacing[1] + spacing[1]/2.0;
  center[2] = origin[2] + k * spacing[2] + spacing[2]/2.0;
}