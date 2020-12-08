TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   SolverControl
QT          +=  core gui widgets
DEFINES     +=  SOLVERCONTROL_API

include(./solverControl.pri)

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
	-L../../output/bin -lModuleBase \
	-L../../output/bin -lModelData \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lIO \
	-L../../output/bin -lPostWidgets \
	-L../../output/bin -lConfigOptions \
	-L../../output/bin -lPythonModule \
	-L../../output/bin -lSelfDefObject \
 
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
	-L../../output/bin_d -lSettings \
	-L../../output/bin_d -lModuleBase \
    -L../../output/bin_d -lModelData \
	-L../../output/bin_d -lDataProperty \
	-L../../output/bin_d -lIO \
	-L../../output/bin_d -lPostWidgets \
	-L../../output/bin_d -lConfigOptions \
    -L../../output/bin_d -lPythonModule \
	-L../../output/bin_d -lSelfDefObject \
   
    message("Windows solverControl build")
  
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
	-L../../output/bin -lModuleBase \
	-L../../output/bin -lModelData \
	-L../../output/bin -lDataProperty \
	-L../../output/bin -lIO \
	-L../../output/bin -lPostWidgets \
    -L../../output/bin -lPythonModule \
  
  message("Linux solverControl build")
}

RESOURCES += ../qrc/qianfan.qrc

TRANSLATIONS     +=  ../qrc/translations/SolverControl_Zh_CN.ts
