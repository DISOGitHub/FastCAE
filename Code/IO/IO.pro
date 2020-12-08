TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   IO
QT          +=  core xml widgets 
DEFINES     +=  IO_API

include(./IO.pri)
include(./../vtk.pri)
include(./../occ.pri)

win32{

  INCLUDEPATH	+=  ./ \
                    ../ \
					../../quazip/include/ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
  -L../../output/bin -lMeshData \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lModuleBase \
  -L../../output/bin -lGeometry \
  -L../../output/bin -lModelData \
  -L../../output/bin -lPluginManager \
  -L../../output/bin -lSettings \
  -L../../output/bin -lMaterial \
  -L../../quazip/lib -lquazip \
  -L../../output/bin -lGmshModule \
 
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lMeshData \
  -L../../output/bin_d -lDataProperty \
  -L../../output/bin_d -lModuleBase \
  -L../../output/bin_d -lGeometry \
  -L../../output/bin_d -lModelData \
  -L../../output/bin_d -lPluginManager \
  -L../../output/bin_d -lSettings \
  -L../../output/bin_d -lMaterial \
  -L../../quazip/libd -lquazipd  \
  -L../../output/bin_d -lGmshModule \
  
  message("Windows IO build")
  
}


unix{
    INCLUDEPATH	+=   ./ \
		             ../ \
					 ../../quazip/include/quazip \
  CONFIG          += plugin
  DESTDIR         = ../../output/bin
  MOC_DIR         = ./release/moc
  UI_DIR          = ./release/qui
  RCC_DIR         = ./release/rcc
  OBJECTS_DIR     = ./release/obj
  LIBS += \
  -L../../output/bin -lMeshData \
  -L../../output/bin -lDataProperty \
  -L../../output/bin -lModuleBase \
  -L../../output/bin -lGeometry \
  -L../../output/bin -lModelData \
  -L../../output/bin -lSettings \	
  -L../../quazip/lib -lquazip \
  
  message("Linux IO build")
  
}

TRANSLATIONS     += ../qrc/translations/IO_zh_CN.ts
