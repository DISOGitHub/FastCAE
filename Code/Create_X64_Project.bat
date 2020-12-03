
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 10.0.17763.0

SET "PATH=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\;C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64;%PATH%"  
 
cd BCBase 
qmake CONFIG+=X64 -tp vc BCBase.pro 
  
cd ..\ConfigOptions 
qmake CONFIG+=X64 -tp vc ConfigOptions.pro 
  
cd ..\CurveAnalyse 
qmake CONFIG+=X64 -tp vc CurveAnalyse.pro 
  
cd ..\DataProperty 
qmake CONFIG+=X64 -tp vc DataProperty.pro 
  
cd ..\geometry 
qmake CONFIG+=X64 -tp vc geometry.pro 
  
cd ..\GeometryCommand 
qmake CONFIG+=X64 -tp vc geometryCommand.pro 
  
cd ..\GeometryDataExchange 
qmake CONFIG+=X64 -tp vc GeometryDataExchange.pro 
  
cd ..\GeometryWidgets 
qmake CONFIG+=X64 -tp vc geometryWidgets.pro 
  
cd ..\Gmsh 
qmake CONFIG+=X64 -tp vc GmshModule.pro 
  
cd ..\GraphicsAnalyse 
qmake CONFIG+=X64 -tp vc GraphicsAnalyse.pro 
  
cd ..\HeuDataSrcIO 
qmake CONFIG+=X64 -tp vc HeuDataSrcIO.pro 
  
cd ..\IO 
qmake CONFIG+=X64 -tp vc IO.pro 
  
cd ..\License 
qmake CONFIG+=X64 -tp vc License.pro 
  
cd ..\main 
qmake CONFIG+=X64 -tp vc main.pro 
  
cd ..\MainWidgets 
qmake CONFIG+=X64 -tp vc mainwidgets.pro 
  
cd ..\mainWindow 
qmake CONFIG+=X64 -tp vc mainWindow.pro 
  
cd ..\Material 
qmake CONFIG+=X64 -tp vc Material.pro 
  
cd ..\meshData 
qmake CONFIG+=X64 -tp vc meshData.pro 
  
cd ..\ModelData 
qmake CONFIG+=X64 -tp vc modelData.pro 
  
cd ..\moduleBase 
qmake CONFIG+=X64 -tp vc moduleBase.pro 
  
cd ..\ParaClassFactory 
qmake CONFIG+=X64 -tp vc ParaClassFactory.pro 
  
cd ..\Plugin2DMeshTools 
qmake CONFIG+=X64 -tp vc 2DMeshTools.pro 
  
cd ..\PluginComplex 
qmake CONFIG+=X64 -tp vc ComplexPlugin.pro 
  
cd ..\PluginCustomizer 
qmake CONFIG+=X64 -tp vc pluginCustomizer.pro 
  
cd ..\PluginHull 
qmake CONFIG+=X64 -tp vc HullCutPlugin.pro 
  
cd ..\PluginMachinery 
qmake CONFIG+=X64 -tp vc MachineryPlugin.pro 
  
cd ..\PluginManager 
qmake CONFIG+=X64 -tp vc PluginManager.pro 
  
cd ..\PluginMeshDataExchange 
qmake CONFIG+=X64 -tp vc meshDataExchangePlugin.pro 
  
cd ..\PluginModel 
qmake CONFIG+=X64 -tp vc pluginModel.pro 
  
cd ..\PluginThermalFluid 
qmake CONFIG+=X64 -tp vc ThermalFluid.pro 
  
cd ..\PostWidgets 
qmake CONFIG+=X64 -tp vc PostWidgets.pro 
  
cd ..\ProjectTree 
qmake CONFIG+=X64 -tp vc projectTree.pro 
  
cd ..\ProjectTreeExtend 
qmake CONFIG+=X64 -tp vc ProjectTreeExtend.pro 
  
cd ..\python 
qmake CONFIG+=X64 -tp vc python.pro 
  
cd ..\SelfDefObject 
qmake CONFIG+=X64 -tp vc SelfDefObject.pro 
  
cd ..\settings 
qmake CONFIG+=X64 -tp vc setting.pro 
  
cd ..\SolutionDataIO 
qmake CONFIG+=X64 -tp vc SolutionDataIO.pro 
  
cd ..\SolverControl 
qmake CONFIG+=X64 -tp vc solverControl.pro 
  
cd ..\UserGuidence 
qmake CONFIG+=X64 -tp vc UserGuidence.pro 
  
cd ..\XGenerateReport 
qmake CONFIG+=X64 -tp vc XGenerateReport.pro 

cd ..\
copy /y ".\python\py\*.py" "..\output\bin_d"
copy /y ".\python\py\*.py" "..\output\bin"
  
pause  
