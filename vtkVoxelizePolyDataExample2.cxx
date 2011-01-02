#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLImageDataWriter.h>

#include "vtkVoxelizePolyData.h"

int main (int argc, char *argv[])
{
  if(argc != 3)
    {
    cout << "Required arguments: InputFilename(vtp) OutputFilename(vti)" << endl;
    return EXIT_FAILURE;
    }
  
  vtkstd::string inputFilename = argv[1];
  vtkstd::string outputFilename = argv[2];
  
  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  
  vtkSmartPointer<vtkVoxelizePolyData> voxelize = vtkSmartPointer<vtkVoxelizePolyData>::New();
  voxelize->SetInput(reader->GetOutput());
  voxelize->Update();
  
  vtkImageData* voxelized = voxelize->GetOutput();
  cout << "voxelized: " << endl << *voxelized << endl;
  /*
  int extent[6];
  voxelized->GetExtent(extent);
  vtkstd::cout << "Extent: " << extent[0] << " " << extent[1] << vtkstd::endl;
  double v = voxelized->GetScalarComponentAsDouble(0,0,0,0);
  vtkstd::cout << "v: " << v << vtkstd::endl;
  */
  vtkSmartPointer<vtkXMLImageDataWriter> writer = vtkSmartPointer<vtkXMLImageDataWriter>::New();
  writer->SetFileName(outputFilename.c_str());
  writer->SetInput(voxelized);
  writer->Write();
  
  return EXIT_SUCCESS;
}
