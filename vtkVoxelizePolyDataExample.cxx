#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkSphereSource.h>
#include <vtkXMLImageDataWriter.h>

#include "vtkVoxelizePolyData.h"

int main (int argc, char *argv[])
{
  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();

  vtkSmartPointer<vtkVoxelizePolyData> voxelize =
    vtkSmartPointer<vtkVoxelizePolyData>::New();
  voxelize->SetInputConnection(sphereSource->GetOutputPort());
  voxelize->Update();

  vtkSmartPointer<vtkXMLImageDataWriter> writer =
    vtkSmartPointer<vtkXMLImageDataWriter>::New();
  writer->SetFileName("Output.vti");
  writer->SetInputConnection(voxelize->GetOutputPort());
  writer->Write();

  return EXIT_SUCCESS;
}
