set(vtkFiltersPython_LOADED 1)
set(vtkFiltersPython_DEPENDS "vtkCommonCore;vtkCommonExecutionModel;vtkPython;vtkWrappingPythonCore")
set(vtkFiltersPython_LIBRARIES "vtkFiltersPython")
set(vtkFiltersPython_INCLUDE_DIRS "${VTK_INSTALL_PREFIX}/include/vtk-7.1")
set(vtkFiltersPython_LIBRARY_DIRS "")
set(vtkFiltersPython_RUNTIME_LIBRARY_DIRS "${VTK_INSTALL_PREFIX}/bin")
set(vtkFiltersPython_WRAP_HIERARCHY_FILE "${CMAKE_CURRENT_LIST_DIR}/vtkFiltersPythonHierarchy.txt")
set(vtkFiltersPython_KIT "vtkWrapping")

