cmake_minimum_required(VERSION 2.6)

PROJECT(vtkVoxelizePolyData)
ENABLE_TESTING()

SET(BUILD_PARAVIEW_PLUGIN OFF CACHE BOOL "Build Paraview plugin?")

# Paraview is required if we are going to build the plugin, otherwise VTK is all we need.
if(BUILD_PARAVIEW_PLUGIN)
  FIND_PACKAGE(ParaView REQUIRED)
  INCLUDE(${PARAVIEW_USE_FILE})
  add_subdirectory(plugin)
else()
  FIND_PACKAGE(VTK REQUIRED)
  INCLUDE(${VTK_USE_FILE})
endif()

ADD_EXECUTABLE(vtkVoxelizePolyDataExample vtkVoxelizePolyDataExample.cxx
vtkVoxelizePolyData.cxx)
TARGET_LINK_LIBRARIES(vtkVoxelizePolyDataExample ${VTK_LIBRARIES})

ADD_EXECUTABLE(vtkPointSetSurfaceReconstructionExample vtkPointSetSurfaceReconstructionExample.cxx
vtkPointSetSurfaceReconstruction.cxx
vtkVoxelizePolyData.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetSurfaceReconstructionExample ${VTK_LIBRARIES})

