TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   ModuleBase
QT          +=  core widgets gui xml printsupport
DEFINES     +=  MODULEBASE_API

include(./moduleBase.pri)
include(./../vtk.pri)
include(./../occ.pri)

win32{

  INCLUDEPATH	+=  ./ \
                    ../ \
					../SelfDefObject/ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
  -L../../output/bin -lMeshData \
  -L../../output/bin -lGeometry \
 # -L../../output/bin -lGraphicsAnalyse \
  -L../../output/bin -lModelData \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lSelfDefObject \
  -L../../output/bin -lSettings \
  -L../../output/bin -lPythonModule \
#  -L../../output/bin -lXMaterial \
 
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lMeshData \
  -L../../output/bin_d -lGeometry \
 # -L../../output/bin_d -lGraphicsAnalyse \
  -L../../output/bin_d -lModelData \
  -L../../output/bin_d -lDataProperty \
  -L../../output/bin_d -lSelfDefObject \
  -L../../output/bin_d -lSettings \
  -L../../output/bin_d -lPythonModule \
#  -L../../output/bin_d -lXMaterial \
#  -L../../output/bin_d -lMainWindow \
  
  message("Windows moduleBase build")
  
}


unix{
    INCLUDEPATH	+=   ./ \
		      ../ \
			  ../SelfDefObject/ \
					 
  CONFIG          += plugin
  DESTDIR         = ../../output/bin
  MOC_DIR         = ./release/moc
  UI_DIR          = ./release/qui
  RCC_DIR         = ./release/rcc
  OBJECTS_DIR     = ./release/obj
  LIBS += \
  -L../../output/bin -lMeshData \
  -L../../output/bin -lGeometry \
#  -L../../output/bin -lGraphicsAnalyse \
  -L../../output/bin -lModelData \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lSelfDefObject \
  -L../../output/bin -lSettings \
  -L../../output/bin -lPythonModule \
#  -L../../output/bin -lXMaterial \
	
  message("Linux moduleBase build")
  
}

RESOURCES += ../qrc/qianfan.qrc
RESOURCES += ../qrc/translations.qrc
#RESOURCES += ../qrc/config.qrc

TRANSLATIONS     += ../qrc/translations/ModuleBase_zh_CN.ts

