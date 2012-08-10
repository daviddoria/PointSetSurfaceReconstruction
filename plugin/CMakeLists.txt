INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ../)

ADD_PARAVIEW_PLUGIN(VoxelizePolyData "1.0"
SERVER_MANAGER_XML vtkVoxelizePolyData.xml
SERVER_MANAGER_SOURCES ../vtkVoxelizePolyData.cxx
)

ADD_PARAVIEW_PLUGIN(PointSetSurfaceReconstruction "1.0"
SERVER_MANAGER_XML vtkPointSetSurfaceReconstruction.xml
SERVER_MANAGER_SOURCES ../vtkPointSetSurfaceReconstruction.cxx ../vtkVoxelizePolyData.cxx)
