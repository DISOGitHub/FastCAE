TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   PluginThermalFluid
QT          +=  core xml gui widgets
DEFINES     +=  ThERMALFLUID_API

include(./ThermalFluid.pri)
include(../vtk.pri)

win32{

  INCLUDEPATH	+=  ./ \
                    ../ \
					../SelfDefObject \

  Release:DESTDIR         = ../../output/bin/plugins 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
  -L../../output/bin -lMeshData \
  -L../../output/bin -lModelData \
  -L../../output/bin -lModuleBase \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lSettings \
  -L../../output/bin -lSelfDefObject \
  -L../../output/bin -lMainWidgets \
  -L../../output/bin -lProjectTree \
  -L../../output/bin -lPluginManager \
  -L../../output/bin -lMainWindow \
	 
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d/plugins
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lMeshData \
  -L../../output/bin_d -lModelData \
  -L../../output/bin_d -lModuleBase \
  -L../../output/bin_d -lDataProperty \
  -L../../output/bin_d -lSettings \
  -L../../output/bin_d -lSelfDefObject \
  -L../../output/bin_d -lMainWidgets \
  -L../../output/bin_d -lProjectTree \
  -L../../output/bin_d -lPluginManager \
  -L../../output/bin_d -lMainWindow \
   
  message("Windows ThermalFluid build")
  
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

RESOURCES += ./resource/TFCoupling.qrc \

TRANSLATIONS += ./resource/TFCoupling_zh_CN.ts




