// .NAME vtkPointSetNormalOrientation - Estimate normals of a point set using a local best fit plane.
// .SECTION Description
// This filter implements the technique described by Hoppe et al.
// in ``Surface reconstruction from unorganized points''. This method
// constructs a nearest neighbor graph on the points. As a good guess
// at ``outside'', the technique finds the point with the largest z
// value and adjusts that point's normal to point more toward the positive
// z direction. This initial normal direction is propagated over a minimum
// spanning tree computed over the graph.

#ifndef __vtkPointSetSurfaceReconstruction_h
#define __vtkPointSetSurfaceReconstruction_h

#include "vtkSmartPointer.h"
#include "vtkPolyDataAlgorithm.h" //superclass

class vtkImageData;

class vtkPointSetSurfaceReconstruction : public vtkPolyDataAlgorithm
{

  public:
    static vtkPointSetSurfaceReconstruction *New();
    vtkTypeMacro(vtkPointSetSurfaceReconstruction, vtkPolyDataAlgorithm);
    void PrintSelf(ostream &os, vtkIndent indent);

    vtkSetMacro(SamplesPerDimension, unsigned int);

    vtkSetMacro(Border, unsigned int);

  protected:
    vtkPointSetSurfaceReconstruction();
    ~vtkPointSetSurfaceReconstruction(){}
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  private:
    unsigned int SamplesPerDimension; // This is used to determine how densely to sample the volume
    unsigned int Border; // Used to set the border of the voxel grid
};

// Helper functions
double SignedDistanceToPlane(double x[3], double n[3], double p0[3]);
void GetCellCenter(vtkImageData* image, unsigned int i, unsigned int j, unsigned int k, double center[3]);

#endif

