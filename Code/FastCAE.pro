TEMPLATE = subdirs

#FastCAE essential project 

SUBDIRS += $$PWD/python/python.pro \
           $$PWD/settings/setting.pro \
           $$PWD/DataProperty/DataProperty.pro \
		   $$PWD/meshData/meshData.pro \
		   $$PWD/SelfDefObject/SelfDefObject.pro \
		   $$PWD/Material/Material.pro \
		   $$PWD/BCBase/BCBase.pro \
		   $$PWD/SolutionDataIO/SolutionDataIO.pro \
		   $$PWD/ConfigOptions/ConfigOptions.pro \
		   $$PWD/ParaClassFactory/ParaClassFactory.pro \
		   $$PWD/HeuDataSrcIO/HeuDataSrcIO.pro \
		   $$PWD/geometry/geometry.pro \
		   $$PWD/ModelData/modelData.pro \
		   $$PWD/CurveAnalyse/CurveAnalyse.pro \
		   $$PWD/moduleBase/moduleBase.pro \
		   $$PWD/GraphicsAnalyse/GraphicsAnalyse.pro \
		   $$PWD/PostWidgets/PostWidgets.pro \
		   $$PWD/GeometryDataExchange/GeometryDataExchange.pro \
		   $$PWD/IO/IO.pro \
		   $$PWD/ProjectTree/projectTree.pro \
		   $$PWD/GeometryCommand/geometryCommand.pro \
		   $$PWD/SolverControl/solverControl.pro \
		   $$PWD/ProjectTreeExtend/ProjectTreeExtend.pro \
		   $$PWD/GeometryWidgets/geometryWidgets.pro \
		   $$PWD/MainWidgets/mainwidgets.pro \
		   $$PWD/PluginManager/PluginManager.pro \
		   $$PWD/Gmsh/GmshModule.pro \
		   $$PWD/UserGuidence/UserGuidence.pro \
		   $$PWD/mainWindow/mainWindow.pro \
		   $$PWD/main/main.pro \
		   
#FastCAE Plugins 

SUBDIRS += $$PWD/PluginMeshDataExchange/meshDataExchangePlugin.pro \
		   $$PWD/PluginCustomizer/pluginCustomizer.pro \
		   
#other Plugins 

#SUBDIRS += $$PWD/PluginMeshDataExchange/meshDataExchangePlugin.pro \

CONFIG += ordered