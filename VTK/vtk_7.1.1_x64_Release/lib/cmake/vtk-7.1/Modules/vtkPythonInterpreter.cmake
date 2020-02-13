set(vtkPythonInterpreter_LOADED 1)
set(vtkPythonInterpreter_DEPENDS "vtkCommonCore;vtkPython;vtksys")
set(vtkPythonInterpreter_LIBRARIES "vtkPythonInterpreter")
set(vtkPythonInterpreter_INCLUDE_DIRS "${VTK_INSTALL_PREFIX}/include/vtk-7.1")
set(vtkPythonInterpreter_LIBRARY_DIRS "")
set(vtkPythonInterpreter_RUNTIME_LIBRARY_DIRS "${VTK_INSTALL_PREFIX}/bin")
set(vtkPythonInterpreter_WRAP_HIERARCHY_FILE "${CMAKE_CURRENT_LIST_DIR}/vtkPythonInterpreterHierarchy.txt")
set(vtkPythonInterpreter_KIT "vtkWrapping")

