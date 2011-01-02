#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"

#include "vtkPointSetSurfaceReconstruction.h"

int main (int argc, char *argv[])
{
  //verify command line arguments
  if(argc != 3)
    {
    cout << "Required arguments: InputFilename.vtp OutputFilename.vti" << endl;
    return EXIT_FAILURE;
    }
  
  //parse command line arguments
  vtkstd::string inputFilename = argv[1];
  vtkstd::string outputFilename = argv[2];
  
  //read input file
  vtkSmartPointer<vtkXMLPolyDataReader> reader = 
      vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  
  //perform surface reconstruction
  vtkSmartPointer<vtkPointSetSurfaceReconstruction> surfaceReconstructionFilter = 
      vtkSmartPointer<vtkPointSetSurfaceReconstruction>::New();
  surfaceReconstructionFilter->SetInput(reader->GetOutput());
  //surfaceReconstructionFilter->SetSamplesPerDimension(100);
  //surfaceReconstructionFilter->SetBorder(5);
  surfaceReconstructionFilter->Update();
  
  vtkPolyData* surface = surfaceReconstructionFilter->GetOutput();
  
  //write the surface to a file
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = 
      vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(outputFilename.c_str());
  writer->SetInput(surface);
  writer->Write();
    
  return EXIT_SUCCESS;
}
