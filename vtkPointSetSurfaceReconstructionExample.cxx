#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"

#include "vtkPointSetSurfaceReconstruction.h"

int main (int argc, char *argv[])
{
  // Verify command line arguments
  if(argc != 3)
    {
    std::cout << "Required arguments: InputFilename.vtp OutputFilename.vtp" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  // Read input file
  vtkSmartPointer<vtkXMLPolyDataReader> reader =
    vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  // Perform surface reconstruction
  vtkSmartPointer<vtkPointSetSurfaceReconstruction> surfaceReconstructionFilter =
    vtkSmartPointer<vtkPointSetSurfaceReconstruction>::New();
  surfaceReconstructionFilter->SetInputData(reader->GetOutput());
  //surfaceReconstructionFilter->SetSamplesPerDimension(100);
  //surfaceReconstructionFilter->SetBorder(5);
  surfaceReconstructionFilter->Update();

  // Write the surface to a file
  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(outputFilename.c_str());
  writer->SetInputConnection(surfaceReconstructionFilter->GetOutputPort());
  writer->Write();

  return EXIT_SUCCESS;
}
