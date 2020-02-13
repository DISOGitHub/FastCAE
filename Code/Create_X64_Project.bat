call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64\vcvars64.bat"

SET "PATH=C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin;%PATH%"

cd main
qmake CONFIG+=X64 -tp vc main.pro

cd ../mainWindow
qmake CONFIG+=X64 -tp vc mainWindow.pro

cd ../MainWidgets
qmake CONFIG+=X64 -tp vc mainwidgets.pro

cd ../moduleBase
qmake CONFIG+=X64 -tp vc moduleBase.pro

cd ../settings
qmake CONFIG+=X64 -tp vc setting.pro

cd ../IO
qmake CONFIG+=X64 -tp vc IO.pro

cd ../geometry
qmake CONFIG+=X64 -tp vc geometry.pro

cd ../meshData
qmake CONFIG+=X64 -tp vc meshData.pro

cd ../BCBase
qmake CONFIG+=X64 -tp vc BCBase.pro

cd ../ModelData
qmake CONFIG+=X64 -tp vc modeldata.pro

cd ../DataProperty
qmake CONFIG+=X64 -tp vc DataProperty.pro

cd ../SolverControl
qmake CONFIG+=X64 -tp vc solverControl.pro

cd ../SelfDefObject
qmake CONFIG+=X64 -tp vc SelfDefObject.pro

cd ../PostWidgets
qmake CONFIG+=X64 -tp vc PostWidgets.pro

cd ../SolutionDataIO
qmake CONFIG+=X64 -tp vc SolutionDataIO.pro

cd ../CurveAnalyse
qmake CONFIG+=X64 -tp vc CurveAnalyse.pro

cd ../GraphicsAnalyse
qmake CONFIG+=X64 -tp vc GraphicsAnalyse.pro

cd ../HeuDataSrcIO
qmake CONFIG+=X64 -tp vc HeuDataSrcIO.pro

cd ../XGenerateReport
qmake CONFIG+=X64 -tp vc XGenerateReport.pro

cd ../Material
qmake CONFIG+=X64 -tp vc Material.pro

cd ../License
qmake CONFIG+=X64 -tp vc License.pro

cd ../ProjectTree
qmake CONFIG+=X64 -tp vc projectTree.pro

cd ../ProjectTreeExtend
qmake CONFIG+=X64 -tp vc ProjectTreeExtend.pro

cd ../ParaClassFactory
qmake CONFIG+=X64 -tp vc ParaClassFactory.pro

cd ../ConfigOptions
qmake CONFIG+=X64 -tp vc ConfigOptions.pro

cd ../python
qmake CONFIG+=X64 -tp vc python.pro

cd ../GeometryCommand
qmake CONFIG+=X64 -tp vc geometryCommand.pro

cd ../GeometryWidgets
qmake CONFIG+=X64 -tp vc geometryWidgets.pro

cd ../PluginManager
qmake CONFIG+=X64 -tp vc PluginManager.pro

cd ../Gmsh

qmake CONFIG+=X64 -tp vc GmshModule.pro
mkdir ..\..\output\bin_d\gmsh
mkdir ..\..\output\bin\gmsh
copy /y ".\gmsh441\gmsh.exe" "..\..\output\bin_d\gmsh"
copy /y ".\gmsh441\gmsh.exe" "..\..\output\bin\gmsh"
copy /y ".\gmsh441\gmsh.Geo" "..\..\output\bin_d\gmsh"
copy /y ".\gmsh441\gmsh.Geo" "..\..\output\bin\gmsh"


pause
