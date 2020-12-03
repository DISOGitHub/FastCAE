TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   GraphicsAnalyse
QT          +=  core gui widgets opengl xml
DEFINES     +=  GRAPHICSANALYSE_LIBRARY
#DEFINES  += PATHON_SCRIPT
include(./GraphicsAnalyse.pri)
include(./../vtk.pri)

win32{

LIBS += -lOpenGL32  \

LIBS += -lGlU32  \

  INCLUDEPATH	+=  ./ \
                    ../ \
					./CTK \
					./CTK/Libs/Visualization/VTK/Widgets \
					./CTK/Libs/Widgets \
					./CTK/Libs/Visualization/VTK/Core \
					./CTK/Libs/Core \
					../HeuDataSrcIO \
					../SolutionDataIO \
					../CurveAnalyse \
					
					

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS	+=	\
  -L../../output/bin -lHeuDataSrcIO \
  -L../../output/bin -lSolutionDataIO \
  -L../../output/bin -lCurveAnalyse \
  #-L../../output/bin -lPythonModule \
  
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lHeuDataSrcIO \
  -L../../output/bin_d -lSolutionDataIO \
  -L../../output/bin_d -lCurveAnalyse \
  #-L../../output/bin_d -lPythonModule \
	
  message("Windows GraphicsAnalyse build")
                                               
}

unix{
  INCLUDEPATH	+=   ./ \
		             ../ \
					 ../HeuDataSrcIO \
					 ../CurveAnalyse \
					 ./CTK \
					 ./CTK/Libs/Visualization/VTK/Widgets \
					 ./CTK/Libs/Visualization/VTK/Core \
					 ./CTK/Libs/Core \
					 ./CTK/Libs/Widgets \
					 ../HeuDataSrcIO \
					../SolutionDataIO \
					../CurveAnalyse \
					../pathon \
					 
  CONFIG          += plugin
  DESTDIR         = ../../output/bin
  MOC_DIR         = ./release/moc
  UI_DIR          = ./release/qui
  RCC_DIR         = ./release/rcc
  OBJECTS_DIR     = ./release/obj
  LIBS += \
	-L../../output/bin -lHeuDataSrcIO \
	-L../../output/bin -lSolutionDataIO \
	-L../../output/bin -lCurveAnalyse \
	#-L../../output/bin -lPythonModule \
	
  message("Linux GraphicsAnalyse build")
}
RESOURCES += ./resource.qrc

TRANSLATIONS += ../translations/GraphicsAnalyse_zh_CN.ts