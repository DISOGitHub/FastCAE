TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   ModelData
QT          +=  core xml gui widgets
DEFINES     +=  MODELDATA_API

include(./modelData.pri)

win32{

  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lSettings \
	-L../../output/bin -lBCBase \
	-L../../output/bin -lConfigOptions \
	-L../../output/bin -lParaClassFactory \
	-L../../output/bin -lPythonModule \
	-L../../output/bin -lGeometry \
	-L../../output/bin -lMeshData \
	
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
	-L../../output/bin_d -lDataProperty \
	-L../../output/bin_d -lSettings \
	-L../../output/bin_d -lBCBase \
	-L../../output/bin_d -lConfigOptions \
	-L../../output/bin_d -lParaClassFactory \
	-L../../output/bin_d -lPythonModule \
    -L../../output/bin_d -lGeometry \
	-L../../output/bin_d -lMeshData \
	
  message("Windows modelData build")
  
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
	-L../../output/bin -lSettings \
	-L../../output/bin -lBCBase \
	-L../../output/bin -lConfigOptions \
	-L../../output/bin -lParaClassFactory \
	-L../../output/bin -lPythonModule \
	
  message("Linux modelData build")
  
}

