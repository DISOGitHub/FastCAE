TEMPLATE 	= 	lib
TARGET 		= 	License
QT          +=  core gui widgets xml
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
DEFINES 	+=  LICENSE_LIB

include(License.pri)

QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings

win32{
  INCLUDEPATH	+=  ./ \
                    ../ \
					../Setting/ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS			  +=  \
  -L../../output/bin -lSettings \

  
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj
  Debug:LIBS			+=  \
  -L../../output/bin_d -lSettings \
	
  message("Windows License build")
                                               
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
   -L../../output/bin -lMainWindow \

	
  message("Linux License build")
}