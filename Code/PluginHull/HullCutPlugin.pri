HEADERS += \
	$$PWD/HullPluginAPI.h \
	$$PWD/HullCutPlugin.h \
	$$PWD/actionHullCutManager.h \
	$$PWD/dialogShipTree.h \
	$$PWD/GeoCommandMakeHullCut.h \
	$$PWD/dialogModelImport.h \
    $$PWD/dialogConfigWind.h \
	$$PWD/KitShipTreeItemType.h \
    $$PWD/dialogConfigWave.h \
    $$PWD/dialogConfigOceanCurrent.h \
    $$PWD/dialogConfigWaveSpectrum.h \
	$$PWD/dialogHullCut.h \
	$$PWD/dialogSingleCurve.h \
	$$PWD/dialogInputData.h \
	$$PWD/ModelPoint.h \
	$$PWD/cleaner.hpp	\
	
SOURCES += \
	$$PWD/HullCutPlugin.cpp \
	$$PWD/actionHullCutManager.cpp \
	$$PWD/dialogShipTree.cpp \
	$$PWD/GeoCommandMakeHullCut.cpp \
	$$PWD/dialogModelImport.cpp \
    $$PWD/dialogConfigWind.cpp \
    $$PWD/dialogConfigWave.cpp \
    $$PWD/dialogConfigOceanCurrent.cpp \
    $$PWD/dialogConfigWaveSpectrum.cpp \
	$$PWD/dialogHullCut.cpp \
	$$PWD/dialogSingleCurve.cpp \
	$$PWD/dialogInputData.cpp \
	$$PWD/ModelPoint.cpp \
	
FORMS +=  \
	$$PWD/dialogHullCut.ui \
	$$PWD/dialogModelImport.ui \
    $$PWD/dialogConfigWind.ui \
    $$PWD/dialogConfigWave.ui \
    $$PWD/dialogConfigOceanCurrent.ui \
    $$PWD/dialogConfigWaveSpectrum.ui \
	$$PWD/dialogSingleCurve.ui \
	$$PWD/dialogInputData.ui \

RESOURCES += \
    $$PWD/hullcutplugin.qrc