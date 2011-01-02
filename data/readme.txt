Input.vtp is a set of points sampled from a sphere to use as the input to vtkPointSetSurfaceReconstructionExample.cxx
GroundTruthOutput.vtp is the correct resulting surface produced by vtkPointSetSurfaceReconstructionExample.cxx. This is provided as a sanity check. You should run:
./vtkPointSetSurfaceReconstructionExample Input.vtp Output.vtp
and then Output.vtp should match GroundTruthOutput.vtp