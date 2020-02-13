TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   ProjectTree
QT          +=  core gui widgets xml
DEFINES     +=  PROJECTTREE_API

include(./projectTree.pri)
include(./../vtk.pri)

win32{

  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
    -L../../output/bin -lModuleBase \
	-L../../output/bin -lModelData \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lMeshData \
	-L../../output/bin -lGeometry \
	-L../../output/bin -lSelfDefObject \
	-L../../output/bin -lBCBase \
	-L../../output/bin -lParaClassFactory \
	-L../../output/bin -lSettings \
	-L../../output/bin -lConfigOptions \
	-L../../output/bin -lMaterial \
	-L../../output/bin -lPostWidgets \
	-L../../output/bin -lPythonModule \
 
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
   -L../../output/bin_d -lModuleBase \
   -L../../output/bin_d -lModelData \
   -L../../output/bin_d -lDataProperty \
   -L../../output/bin_d -lMeshData \
   -L../../output/bin_d -lGeometry \
   -L../../output/bin_d -lSelfDefObject \
   -L../../output/bin_d -lBCBase \
   -L../../output/bin_d -lParaClassFactory \
   -L../../output/bin_d -lSettings \
   -L../../output/bin_d -lConfigOptions \
   -L../../output/bin_d -lMaterial \
   -L../../output/bin_d -lPostWidgets \
   -L../../output/bin_d -lPythonModule \
  
  message("Windows projectTreeBase build")
  
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
	-L../../output/bin -lModuleBase \
	-L../../output/bin -lModelData \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lMeshData \
	-L../../output/bin -lGeometry \
	-L../../output/bin -lSelfDefObject \
	-L../../output/bin -lBCBase \
	-L../../output/bin -lParaClassFactory \
	-L../../output/bin -lSettings \
	-L../../output/bin -lConfigOptions \
	-L../../output/bin -lMaterial \
	-L../../output/bin -lPythonModule \
	
  message("Linux projectTreeBase build")
  
}

RESOURCES += ../qrc/qianfan.qrc
#RESOURCES += ../qrc/config.qrc

TRANSLATIONS     += ../qrc/translations/ProjectTree_zh_CN.ts

