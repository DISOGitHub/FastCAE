TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   PluginPHengLEI
QT          +=  core xml gui widgets
DEFINES     +=  PHENGLEIPLUGIN_API

include(./pluginPHengLEI.pri)
include(../vtk.pri)

win32{

  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin/plugins 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
  -L../../output/bin -lMeshData \
  -L../../output/bin -lGeometry \
  -L../../output/bin -lModelData \
  -L../../output/bin -lModuleBase \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lIO \
  -L../../output/bin -lSettings \
  -L../../output/bin -lSelfDefObject \
  -L../../output/bin -lSolverControl \
  -L../../output/bin -lMainWidgets \
#  -L../../output/bin -lXMaterial \
  -L../../output/bin -lProjectTree \
#  -L../../output/bin -lGenerateGeometry \
  -L../../output/bin -lGeometryCommand \
  -L../../output/bin -lConfigOptions \
  -L../../output/bin -lProjectTreeExtend \
  -L../../output/bin -lMaterial \
  -L../../output/bin -lPythonModule \
  -L../../output/bin -lPostWidgets \
  -L../../output/bin -lGeometryWidgets \
  -L../../output/bin -lPluginManager \
   -L../../output/bin -lBCBase \
 
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d/plugins
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lMeshData \
  -L../../output/bin_d -lGeometry \
  -L../../output/bin_d -lModelData \
  -L../../output/bin_d -lModuleBase \
  -L../../output/bin_d -lDataProperty \
  -L../../output/bin_d -lIO \
  -L../../output/bin_d -lSettings \
  -L../../output/bin_d -lSelfDefObject \
  -L../../output/bin_d -lSolverControl \
  -L../../output/bin_d -lMainWidgets \
#  -L../../output/bin_d -lXMaterial \
  -L../../output/bin_d -lProjectTree \
 # -L../../output/bin_d -lGenerateGeometry \
  -L../../output/bin_d -lGeometryCommand \
  -L../../output/bin_d -lConfigOptions \
  -L../../output/bin_d -lProjectTreeExtend \
  -L../../output/bin_d -lMaterial \
  -L../../output/bin_d -lPythonModule \
  -L../../output/bin_d -lPostWidgets \
  -L../../output/bin_d -lGeometryWidgets \
  -L../../output/bin_d -lPluginManager \
  -L../../output/bin_d -lBCBase \
   
  message("Windows pluginModel build")
  
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
  -L../../output/bin -lDataProperty \
	
  message("Linux meshdata build")
  
}

