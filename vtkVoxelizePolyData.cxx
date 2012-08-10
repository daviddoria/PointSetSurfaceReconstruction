#include "vtkVoxelizePolyData.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkPolyData.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"

//for testing only
#include "vtkXMLImageDataWriter.h"

vtkStandardNewMacro(vtkVoxelizePolyData);

vtkVoxelizePolyData::vtkVoxelizePolyData()
{
  this->NumberOfCellsX = 10;
  this->NumberOfCellsY = 10;
  this->NumberOfCellsZ = 10;
}

void vtkVoxelizePolyData::SetNumberOfCellsAll(unsigned int num)
{
  this->NumberOfCellsX = num;
  this->NumberOfCellsY = num;
  this->NumberOfCellsZ = num;
}

//----------------------------------------------------------------------------
int vtkVoxelizePolyData::FillInputPortInformation(
                                            int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
  return 1;
}

int vtkVoxelizePolyData::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // Get the input and ouptut
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkPolyData *input = vtkPolyData::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkImageData *output = vtkImageData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Get the bounds of the input PolyData
  double bounds[6];
  input->GetBounds(bounds);
  //xmin, xmax, ymin, ymax, zmin, zmax
  double xmin = bounds[0];
  double xmax = bounds[1];
  double ymin = bounds[2];
  double ymax = bounds[3];
  double zmin = bounds[4];
  double zmax = bounds[5];

  // Create a grid the size of the point set
  vtkSmartPointer<vtkImageData> grid =
    vtkSmartPointer<vtkImageData>::New();

  /*
  //exactly the right size

  //set the bottom left corner of the grid to the bottom left corner of the data bounding box
  grid->SetOrigin(xmin, ymin, zmin);

  //create a grid of voxels of size (NumberOfCellsX x NumberOfCellsY x NumberOfCellsZ)
  //there are (NumberOfCellsX+1 x NumberOfCellsY+1 x NumberOfCellsZ+1) points to make this size voxel grid
  grid->SetExtent(0, this->NumberOfCellsX, 0, this->NumberOfCellsY, 0, this->NumberOfCellsZ);

  //set the size of each element/cell/voxel so that the grid spans the entire input point set volume
  grid->SetSpacing((xmax-xmin)/static_cast<double>(this->NumberOfCellsX), (ymax-ymin)/static_cast<double>(this->NumberOfCellsY), (zmax-zmin)/static_cast<double>(this->NumberOfCellsZ));
  grid->Update();
  */


  // Compute the size of a cell
  double CellSizeX = (xmax-xmin)/static_cast<double>(this->NumberOfCellsX);
  double CellSizeY = (ymax-ymin)/static_cast<double>(this->NumberOfCellsY);
  double CellSizeZ = (zmax-zmin)/static_cast<double>(this->NumberOfCellsZ);

  // Set the bottom left corner of the grid to the bottom left corner of the data bounding box +
  // a factor dependent on the border
  grid->SetOrigin(xmin - static_cast<double>(this->Border) * CellSizeX,
                  ymin - static_cast<double>(this->Border) * CellSizeY,
                  zmin - static_cast<double>(this->Border) * CellSizeZ);

  // Make the voxel grid with a uniform border surrounding it
  grid->SetExtent(0, this->NumberOfCellsX + 2.0 * static_cast<double>(this->Border),
                  0, this->NumberOfCellsY + 2.0 * static_cast<double>(this->Border),
                  0, this->NumberOfCellsZ + 2.0 * static_cast<double>(this->Border));

  // Set the size of each voxel so that the grid spans the entire grid
  grid->SetSpacing(CellSizeX, CellSizeY, CellSizeZ);

  output->ShallowCopy(grid);
  output->SetExtent(grid->GetExtent());

  return 1;
}


//----------------------------------------------------------------------------
void vtkVoxelizePolyData::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
