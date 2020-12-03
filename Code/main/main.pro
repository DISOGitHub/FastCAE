TEMPLATE	=   app
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   FastCAE
QT          +=  core gui widgets

include(./main.pri)
include(../vtk.pri)
include(../occ.pri)

win32{

Release:QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"

  INCLUDEPATH	+=  ./ \
					../ \
					../Licese/ \

# Release:CONFIG	    	+=  console
  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS +=  \
	-L../../output/bin -lMainWindow \
	-L../../output/bin -lLicense \
	-L../../output/bin -lConfigOptions \
	-L../../output/bin -lProjectTree \
	-L../../output/bin -lSettings \
	-L../../output/bin -lPythonModule \
 

  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
	-L../../output/bin_d -lMainWindow \
	-L../../output/bin_d -lLicense \
	-L../../output/bin_d -lConfigOptions \
	-L../../output/bin_d -lProjectTree \
	-L../../output/bin_d -lSettings \
	-L../../output/bin_d -lPythonModule \
	
  message("Windows main build")
}

unix{

  include(../python.pri)

  INCLUDEPATH	+=   ./ \
		     ../ \

  CONFIG       	+=  console
  CONFIG          += plugin
  DESTDIR         = ../../output/bin
  MOC_DIR         = ./release/moc
  UI_DIR          = ./release/qui
  RCC_DIR         = ./release/rcc
  OBJECTS_DIR     = ./release/obj
  LIBS += \
	-L../../output/bin -lPythonModule \
	-L../../output/bin -lSettings \
        -L../../output/bin -lMeshData \
        -L../../output/bin -lMaterial\
        -L../../output/bin -lConfigOptions \
        -L../../output/bin -lHeuDataSrcIO \
        -L../../output/bin -lGeometry \
        -L../../output/bin -lModelData \
        -L../../output/bin -lCurveAnalyse \
        -L../../output/bin -lModuleBase \
        -L../../output/bin -lGraphicsAnalyse \
		-L../../output/bin -lPostWidgets \
		-L../../output/bin -lIO \
		-L../../output/bin -lProjectTree \
		-L../../output/bin -lSolverControl \
		-L../../output/bin -lProjectTreeExtend \
		-L../../output/bin -lMainWidgets \
		-L../../output/bin -lMainWindow \        
	
  message("Linux main build")
}

RESOURCES += ../qrc/qianfan.qrc

RC_FILE += ../qrc/qianfan.rc
