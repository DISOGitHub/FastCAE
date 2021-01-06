TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   ConfigOptions
QT          +=  core xml widgets
DEFINES     +=  CONFIGOPTIONS_API

include(./ConfigOptions.pri)

win32{
  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS	+=	\
#	-L../../output/bin -lProjectTree \
#	-L../../output/bin -lModuleBase \
	-L../../output/bin -lBCBase \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lMaterial \
	-L../../output/bin -lPythonModule \
	-L../../output/bin -lSettings \
  
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
#	-L../../output/bin_d -lProjectTree \
#	-L../../output/bin_d -lModuleBase \
	-L../../output/bin_d -lBCBase \
	-L../../output/bin_d -lDataProperty \
	-L../../output/bin_d -lMaterial \
	-L../../output/bin_d -lPythonModule \
	-L../../output/bin_d -lSettings \
	
  message("Windows config options build")
                                               
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
#	-L../../output/bin -lProjectTree \
#	-L../../output/bin -lModuleBase \
	-L../../output/bin -lBCBase \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lPythonModule \
	-L../../output/bin -lSettings \
 
  message("Linux config options build")
}

#RESOURCES += ../qrc/config.qrc

