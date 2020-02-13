set(vtkIOXMLParser_LOADED 1)
set(vtkIOXMLParser_DEPENDS "vtkCommonCore;vtkCommonDataModel;vtkIOCore;vtkexpat")
set(vtkIOXMLParser_LIBRARIES "vtkIOXMLParser")
set(vtkIOXMLParser_INCLUDE_DIRS "${VTK_INSTALL_PREFIX}/include/vtk-7.1")
set(vtkIOXMLParser_LIBRARY_DIRS "")
set(vtkIOXMLParser_RUNTIME_LIBRARY_DIRS "${VTK_INSTALL_PREFIX}/bin")
set(vtkIOXMLParser_WRAP_HIERARCHY_FILE "${CMAKE_CURRENT_LIST_DIR}/vtkIOXMLParserHierarchy.txt")
set(vtkIOXMLParser_KIT "vtkIO")

