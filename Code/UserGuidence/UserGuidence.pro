TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   UserGuidence
QT          +=  core gui widgets network 
DEFINES     +=  USERGUIDENCE_API

include(UserGuidence.pri)

win32{

  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS	+=	\
    -L../../output/bin -lSettings \
	-L../../output/bin -lPythonModule \
	
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lSettings \
  -L../../output/bin_d -lPythonModule \

  message("Windows USER GUIDENCE build")
                                               
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
  -L../../output/bin -lSettings \
  -L../../output/bin -lPythonModule \
	
  message("Linux USER GUIDENCE build")
}

RESOURCES += ../qrc/qianfan.qrc \

TRANSLATIONS += ../qrc/translations/UserGuidence_zh_CN.ts
