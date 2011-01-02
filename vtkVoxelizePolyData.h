// .NAME vtkVoxelizePolyData -
// .SECTION Description
//

#ifndef __vtkVoxelizePolyData_h
#define __vtkVoxelizePolyData_h

class vtkImageData;
#include "vtkImageAlgorithm.h"

class vtkVoxelizePolyData : public vtkImageAlgorithm
{
public:

  vtkTypeMacro(vtkVoxelizePolyData,vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkVoxelizePolyData *New();

  // Set the number of cells in each dimension
  vtkSetMacro(NumberOfCellsX, unsigned int);
  vtkSetMacro(NumberOfCellsY, unsigned int);
  vtkSetMacro(NumberOfCellsZ, unsigned int);

  // A convenience function which sets
  // NumberOfCellsX,Y,Z all to 'num'
  void SetNumberOfCellsAll(unsigned int num);

  // Set the number of cells bordering the object
  vtkSetMacro(Border, unsigned int);

  virtual int FillInputPortInformation( int port, vtkInformation* info );

protected:
  vtkVoxelizePolyData();
  ~vtkVoxelizePolyData(){}

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkVoxelizePolyData(const vtkVoxelizePolyData&);  // Not implemented.
  void operator=(const vtkVoxelizePolyData&);  // Not implemented.

  unsigned int NumberOfCellsX;
  unsigned int NumberOfCellsY;
  unsigned int NumberOfCellsZ;

  unsigned int Border;
};

#endif
