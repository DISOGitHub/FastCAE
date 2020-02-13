TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   PluginManager
QT          +=  core xml gui widgets
DEFINES     +=  PLUGINMANAGER_API

include(./PluginManager.pri)


win32{

  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin/ 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lModuleBase \
	-L../../output/bin -lSettings \
 
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d/
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
	-L../../output/bin_d -lDataProperty \
	-L../../output/bin_d -lModuleBase \
	-L../../output/bin_d -lSettings \
   
  message("Windows pluginManager build")
  
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
  -L../../output/bin -lModuleBase \
  -L../../output/bin -lModuleBase \
	
  message("Linux meshdata build")
  
}

RESOURCES += ../qrc/qianfan.qrc \

TRANSLATIONS += ../qrc/translations/PluginManager_zh_CN.ts

