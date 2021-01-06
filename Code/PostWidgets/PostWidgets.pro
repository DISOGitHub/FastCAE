TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   PostWidgets
QT          +=  core gui widgets printsupport
DEFINES     +=  POSTWIDGETS_API

include(./PostWidgets.pri)
include(./../vtk.pri)

win32{
  INCLUDEPATH	+=  ./ \
                    ../ \
					../HeuDataSrcIO \
					../GraphicsAnalyse \
					../CurveAnalyse \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS	+=	\
	-L../../output/bin -lHeuDataSrcIO \
	-L../../output/bin -lGraphicsAnalyse \
	-L../../output/bin -lCurveAnalyse \
	-L../../output/bin -lModuleBase \
	-L../../output/bin -lSettings \
	-L../../output/bin -lModelData \
	-L../../output/bin -lConfigOptions \
    -L../../output/bin -lPythonModule \


  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
	-L../../output/bin_d -lHeuDataSrcIO \
	-L../../output/bin_d -lGraphicsAnalyse \
	-L../../output/bin_d -lCurveAnalyse \
	-L../../output/bin_d -lModuleBase \
	-L../../output/bin_d -lSettings \
	-L../../output/bin_d -lModelData \
	-L../../output/bin_d -lConfigOptions \
    -L../../output/bin_d -lPythonModule \
	
  message("Windows post3D build")
                                               
}

unix{
  INCLUDEPATH	+=   ./ \
		             ../ \
					 ../HeuDataSrcIO \
					../GraphicsAnalyse \
					../CurveAnalyse \
					 
  CONFIG          += plugin
  DESTDIR         = ../../output/bin
  MOC_DIR         = ./release/moc
  UI_DIR          = ./release/qui
  RCC_DIR         = ./release/rcc
  OBJECTS_DIR     = ./release/obj
  LIBS += \
    -L../../output/bin -lHeuDataSrcIO \
	-L../../output/bin -lGraphicsAnalyse \
	-L../../output/bin -lCurveAnalyse \
	-L../../output/bin -lModuleBase \
	-L../../output/bin -lConfigOptions \

	
  message("Linux post3D build")
}

TRANSLATIONS += ../qrc/translations/PostWidgets_zh_CN.ts
