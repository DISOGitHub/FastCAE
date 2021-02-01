TEMPLATE     =  lib
CONFIG		 += debug  c++11 
CONFIG		 += qt
TARGET 		 =  SARibbonBar
QT      	 += core gui widgets
DEFINES 	 += SA_RIBBON_BAR_MAKE_LIB #定义此宏将构建库
#DEFINES 	 += SA_RIBBON_DEBUG_HELP_DRAW # 此宏将绘制辅助线用于调试
RESOURCES 	 += resource.qrc
TRANSLATIONS += ../qrc/translations/SARibbonBar_zh_CN.ts
include(./SARibbonBar.pri)

win32{
  INCLUDEPATH	+=  ./ \
                    ../ \

  Release:DESTDIR         = ../../output/bin 
  Release:MOC_DIR         = ./release/moc 
  Release:RCC_DIR         = ./release/rcc
  Release:UI_DIR          = ./release/qui
  Release:OBJECTS_DIR     = ./release/obj
	
  Debug:CONFIG	    	+=  console
  Debug:DESTDIR         = ../../output/bin_d 
  Debug:MOC_DIR         = ./debug/moc 
  Debug:RCC_DIR         = ./debug/rcc 
  Debug:UI_DIR          = ./debug/qui 
  Debug:OBJECTS_DIR     = ./debug/obj 
	
  message("Windows SARibbonBar build")                                            
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
	
  message("Linux SARibbonBar build")
}
