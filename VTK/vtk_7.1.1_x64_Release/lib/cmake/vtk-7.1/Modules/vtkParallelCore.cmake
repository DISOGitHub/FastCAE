set(vtkParallelCore_LOADED 1)
set(vtkParallelCore_DEPENDS "vtkCommonCore;vtkCommonDataModel;vtkCommonSystem;vtkIOLegacy;vtkUtilitiesHashSource;vtksys")
set(vtkParallelCore_LIBRARIES "vtkParallelCore")
set(vtkParallelCore_INCLUDE_DIRS "${VTK_INSTALL_PREFIX}/include/vtk-7.1")
set(vtkParallelCore_LIBRARY_DIRS "")
set(vtkParallelCore_RUNTIME_LIBRARY_DIRS "${VTK_INSTALL_PREFIX}/bin")
set(vtkParallelCore_WRAP_HIERARCHY_FILE "${CMAKE_CURRENT_LIST_DIR}/vtkParallelCoreHierarchy.txt")
set(vtkParallelCore_KIT "vtkParallel")

