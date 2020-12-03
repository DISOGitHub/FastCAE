TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   PluginMachinery
QT          +=  core xml gui widgets
DEFINES     +=  MACHINERYPLUGIN_API

include(./MachineryPlugin.pri)


win32{

  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin/plugins 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lGeometry \
	-L../../output/bin -lGeometryWidgets \
	-L../../output/bin -lGeometryCommand \
	-L../../output/bin -lModuleBase \
	-L../../output/bin -lPluginManager \
 
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d/plugins
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
	-L../../output/bin_d -lDataProperty \
	-L../../output/bin_d -lGeometry \
	-L../../output/bin_d -lGeometryWidgets \
	-L../../output/bin_d -lGeometryCommand \
	-L../../output/bin_d -lModuleBase \
	-L../../output/bin_d -lPluginManager \
   
  message("Windows meshdata build")
  
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

