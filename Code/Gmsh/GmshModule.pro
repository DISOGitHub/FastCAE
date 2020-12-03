TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   GmshModule
QT          +=  core xml gui widgets
DEFINES     +=  GMSH_API 

include(./GmshModule.pri)
include(./../vtk.pri)
include(./../occ.pri)

win32{

  INCLUDEPATH	+=  ./ \
                    ../ \
					./gmsh441/include/ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
  -L../../output/bin -lMeshData \
  -L../../output/bin -lGeometry \
  -L../../output/bin -lPythonModule \
#  -L../../output/bin -lMainWidgets \
  -L../../output/bin -lGeometryWidgets \
  -L../../output/bin -lSettings \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lSelfDefObject \
  -L../../output/bin -lModuleBase \
# -L../../output/bin -lIO \
 
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lMeshData \
  -L../../output/bin_d -lGeometry \
  -L../../output/bin_d -lPythonModule \
#  -L../../output/bin_d -lMainWidgets \
  -L../../output/bin_d -lGeometryWidgets \
  -L../../output/bin_d -lSettings \
  -L../../output/bin_d -lDataProperty \
  -L../../output/bin_d -lSelfDefObject \
  -L../../output/bin_d -lModuleBase \
# -L../../output/bin_d -lIO \
  
  message("Windows GmshModule build")
  
}


unix{
    INCLUDEPATH	+=   ./ \
		      ../ \
					 
  CONFIG          += plugin
  DESTDIR         = ../../output/bin
  MOC_DIR         = ./release/moc
  UI_DIR          = ./release/qui
  RCC_DIR         = ./release/rcc
  OBJECTS_DIR     = ./release/obj
  LIBS += \
  -L../../output/bin -lMeshData \
  -L../../output/bin -lGeometry \
  -L../../output/bin -lPythonModule \
#  -L../../output/bin -lMainWidgets \
  -L../../output/bin -lGeometryWidgets \
  -L../../output/bin -lSettings \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lSelfDefObject \
  -L../../output/bin -lModuleBase \
#  -L../../output/bin -lIO \
  
  message("Linux meshdata build")
  
}

RESOURCES += ../qrc/qianfan.qrc


TRANSLATIONS     += ../qrc/translations/GmshModule_zh_CN.ts
