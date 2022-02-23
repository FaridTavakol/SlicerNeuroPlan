cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(workspace_mesh_gen)

include(GenerateExportHeader)

set (${PROJECT_NAME}_INCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/"
)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkIOLegacy
  vtkIOPLY
  vtkCommonCore
  vtkCommonDataModel
  vtkFiltersCore
  vtkFiltersGeometry
  vtkFiltersSources
  vtkIOXML
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2
  QUIET
)

file(GLOB_RECURSE workspace_mesh_gen_SRCS 
  ${PROJECT_SOURCE_DIR}/*.cpp
  ${PROJECT_SOURCE_DIR}/*.cpp
)


if (NOT VTK_FOUND)
  message("Skipping Point: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_library(workspace_mesh_gen  workspace_mesh_gen.cxx )
  target_link_libraries(workspace_mesh_gen PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_library(workspace_mesh_gen workspace_mesh_gen.cxx )
  target_link_libraries(workspace_mesh_gen PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS workspace_mesh_gen
    MODULES ${VTK_LIBRARIES}
    )
endif ()

generate_export_header(workspace_mesh_gen)

install(TARGETS workspace_mesh_gen EXPORT workspace_mesh_gen
    ARCHIVE DESTINATION lib # static and import libs installed to lib
    LIBRARY DESTINATION lib # .so installed to lib
    RUNTIME DESTINATION bin # .dlls installed to bin
    )

# install directory containing public headers to include/my_lib
install(DIRECTORY include DESTINATION ${PROJECT_SOURCE_DIR}
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# create cmake config to lib/cmake/my_libConfig.cmake
install(EXPORT workspace_mesh_gen
    DESTINATION cmake
    FILE workspace_mesh_genConfig.cmake)

export(EXPORT workspace_mesh_gen FILE workspace_mesh_genConfig.cmake)