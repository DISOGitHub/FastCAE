TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   GeometryCommand
QT          +=  core gui widgets 
DEFINES     +=  GEOMETRYCOMMAND_API

include(./geometryCommand.pri)
include(./../vtk.pri)
include(./../occ.pri)

win32{

  INCLUDEPATH	+=  ./ \
                    ../ \
					../../ \
				

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lGeometry \
  -L../../output/bin -lModuleBase \
  -L../../output/bin -lGeometryDataExchange \
  
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lDataProperty \
  -L../../output/bin_d -lGeometry \
  -L../../output/bin_d -lModuleBase \
  -L../../output/bin_d -lGeometryDataExchange \
						 
  message("Windows geometryCommand build")
  
}


unix{
    INCLUDEPATH	+=   ./ \
		            ../ \
			        ../../ \
					 
  CONFIG          += plugin
  DESTDIR         = ../../output/bin
  MOC_DIR         = ./release/moc
  UI_DIR          = ./release/qui
  RCC_DIR         = ./release/rcc
  OBJECTS_DIR     = ./release/obj
  LIBS += \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lGeometry \
  -L../../output/bin -lModuleBase \
	
  message("Linux geometryCommand build")
  
}

#RESOURCES += ../qrc/qianfan.qrc
#RESOURCES += ../qrc/config.qrc

#TRANSLATIONS     += ../qrc/translations/GeometryWidgets_zh_CN.ts

