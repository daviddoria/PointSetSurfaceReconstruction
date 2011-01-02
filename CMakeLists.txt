cmake_minimum_required(VERSION 2.6)

PROJECT(vtkVoxelizePolyData)
ENABLE_TESTING()

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

SET(BUILD_PARAVIEW_PLUGIN OFF CACHE BOOL "Build Paraview plugin?")

if(BUILD_PARAVIEW_PLUGIN)
 SUBDIRS(plugin)
endif(BUILD_PARAVIEW_PLUGIN)

ADD_EXECUTABLE(vtkVoxelizePolyDataExample vtkVoxelizePolyDataExample.cxx
vtkVoxelizePolyData.cxx)
TARGET_LINK_LIBRARIES(vtkVoxelizePolyDataExample vtkHybrid )

ADD_EXECUTABLE(vtkPointSetSurfaceReconstructionExample vtkPointSetSurfaceReconstructionExample.cxx
vtkPointSetSurfaceReconstruction.cxx
vtkVoxelizePolyData.cxx)
TARGET_LINK_LIBRARIES(vtkPointSetSurfaceReconstructionExample vtkHybrid )

