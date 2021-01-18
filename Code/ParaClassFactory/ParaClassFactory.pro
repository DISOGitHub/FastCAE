TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   ParaClassFactory
QT          +=  core 
DEFINES     +=  PARACLASSFACTORY_API

include(./ParaClassFactory.pri)

win32{
  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS	+=	\
	-L../../output/bin -lBCBase \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lConfigOptions \
  
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
	-L../../output/bin_d -lBCBase \
	-L../../output/bin_d -lDataProperty \
	-L../../output/bin_d -lConfigOptions \
	
  message("Windows ClassFactory build")
                                               
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
	-L../../output/bin -lBCBase \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lConfigOptions \
	
  message("Linux ClassFactory build")
}
