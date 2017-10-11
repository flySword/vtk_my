cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()



PROJECT (myVolumeRendering)
#SET(ENV{VTK_DIR} /home/hadoop/vtk-7.1.1/_bin) 
SET(ENV{VTK_DIR} "D:/Program Files (x86) portable/vtk_7.1.1/_bin")
find_package(VTK REQUIRED)
# 以下可以指定可以不指定
# vtk_module_config(VTK
  # vtkCommonCore
  # vtkFiltersSources
  # vtkInteractionStyle
  # vtkRendering${VTK_RENDERING_BACKEND}
  # vtkIOImage
  # vtkIOXML
  # vtkInteractionWidgets
  # vtkRenderingVolume${VTK_RENDERING_BACKEND}
  # vtkRenderingCore 
# )
include(${VTK_USE_FILE})

SET(SRC_FILES
#  myVolumeRender.cpp
  MyVtkTool.cpp
#  test_imageOutput.cpp
#  hello1.cxx
#  myVolumeRender.cpp
#  ImageSlicing.cxx
#  imageBlend.cpp
#  volume2image.cpp
  imageBlend.cpp
#  blendRender.cpp
)

add_executable(volRend ${SRC_FILES})

#  set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")


target_link_libraries(volRend ${VTK_LIBRARIES})
