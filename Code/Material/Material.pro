TEMPLATE 	= 	lib
TARGET 		= 	Material
QT          +=  core xml gui widgets
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
DEFINES 	+=  MATERIAL_API

include(Material.pri)

win32{
  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
  -L../../output/bin -lSettings \
  -L../../output/bin -lDataProperty \
#  -L../../output/bin -lModuleBase \
  -L../../output/bin -lSelfDefObject \
  -L../../output/bin -lPythonModule \

  
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lSettings \
  -L../../output/bin_d -lDataProperty \
#  -L../../output/bin_d -lModuleBase \
  -L../../output/bin_d -lSelfDefObject \
  -L../../output/bin_d -lPythonModule \
	
  message("Windows Material build")
                                               
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
#   -L../../output/bin -lMainWindow \
  -L../../output/bin -lSettings \
  -L../../output/bin -lDataProperty \
#  -L../../output/bin -lModuleBase \
  -L../../output/bin -lSelfDefObject \
  -L../../output/bin -lPythonModule \
	
  message("Linux XMaterial build")
}

RESOURCES += ../qrc/qianfan.qrc

TRANSLATIONS     += ../qrc/translations/Material_zh_CN.ts