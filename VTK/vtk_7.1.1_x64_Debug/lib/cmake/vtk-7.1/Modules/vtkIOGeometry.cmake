set(vtkIOGeometry_LOADED 1)
set(vtkIOGeometry_DEPENDS "vtkCommonCore;vtkCommonDataModel;vtkCommonExecutionModel;vtkCommonMisc;vtkCommonSystem;vtkCommonTransforms;vtkIOCore;vtkIOLegacy;vtksys;vtkzlib")
set(vtkIOGeometry_LIBRARIES "vtkIOGeometry")
set(vtkIOGeometry_INCLUDE_DIRS "${VTK_INSTALL_PREFIX}/include/vtk-7.1")
set(vtkIOGeometry_LIBRARY_DIRS "")
set(vtkIOGeometry_RUNTIME_LIBRARY_DIRS "${VTK_INSTALL_PREFIX}/bin")
set(vtkIOGeometry_WRAP_HIERARCHY_FILE "${CMAKE_CURRENT_LIST_DIR}/vtkIOGeometryHierarchy.txt")
set(vtkIOGeometry_KIT "vtkIO")

