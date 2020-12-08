TEMPLATE	=   lib
CONFIG		+=  debug  c++11 
CONFIG		+=  qt
TARGET		=   HeuDataSrcIO
QT          +=  core gui widgets
DEFINES     +=  HEUDATASRCIO_LIBRARY

include(./HeuDataSrcIO.pri)
include(./../vtk.pri)
include(./../cgns.pri)
include(./../occ.pri)

win32{
  INCLUDEPATH	+=  ./ \
                    ../ \
					../SolutionDataIO \
					

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
  Release:LIBS	+=	\
  -L../../output/bin -lSolutionDataIO \
  
  
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
  Debug:LIBS +=  \
  -L../../output/bin_d -lSolutionDataIO \

	
  message("Windows HeuDataSrcIO build")
                                               
}

unix{
  INCLUDEPATH	+=   ./ \
		             ../ \
					 ../SolutionDataIO \
					 
  CONFIG          += plugin
  DESTDIR         = ../../output/bin
  MOC_DIR         = ./release/moc
  UI_DIR          = ./release/qui
  RCC_DIR         = ./release/rcc
  OBJECTS_DIR     = ./release/obj
  LIBS += \
  -L../../output/bin -lSolutionDataIO \

	
  message("Linux HeuDataSrcIO build")
}
