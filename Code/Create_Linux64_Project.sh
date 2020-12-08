export PATH=/opt/Qt5.14.2/5.14.2/gcc_64/bin/:$PATH

cd python
qmake -o Makefile python.pro
make ver=release

cd ../settings
qmake -o Makefile setting.pro
make ver=release

cd ../DataProperty
qmake -o Makefile DataProperty.pro
make ver=release

cd ../meshData
qmake -o Makefile meshData.pro
make ver=release

cd ../SelfDefObject
qmake -o Makefile SelfDefObject.pro
make ver=release

cd ../Material
qmake -o Makefile Material.pro
make ver=release

cd ../BCBase
qmake -o Makefile BCBase.pro
make ver=release

cd ../SolutionDataIO
qmake -o Makefile SolutionDataIO.pro
make ver=release

cd ../ConfigOptions
qmake -o Makefile ConfigOptions.pro
make ver=release

cd ../ParaClassFactory
qmake -o Makefile ParaClassFactory.pro
make ver=release

cd ../HeuDataSrcIO
qmake -o Makefile HeuDataSrcIO.pro
make ver=release

cd ../geometry
qmake -o Makefile geometry.pro
make ver=release

cd ../ModelData
qmake -o Makefile modelData.pro
make ver=release

cd ../CurveAnalyse
qmake -o Makefile CurveAnalyse.pro
make ver=release

cd ../moduleBase
qmake -o Makefile moduleBase.pro
make ver=release

cd ../GraphicsAnalyse
qmake -o Makefile GraphicsAnalyse.pro
make ver=release

cd ../PostWidgets
qmake -o Makefile PostWidgets.pro
make ver=release

cd ../GeometryDataExchange
qmake -o Makefile GeometryDataExchange.pro 
make ver=release

cd ../IO
qmake -o Makefile IO.pro
make ver=release

cd ../ProjectTree
qmake -o Makefile projectTree.pro
make ver=release

cd ../GeometryCommand
qmake -o Makefile geometryCommand.pro
make ver=release

cd ../SolverControl
qmake -o Makefile solverControl.pro
make ver=release

cd ../ProjectTreeExtend
qmake -o Makefile ProjectTreeExtend.pro
make ver=release

cd ../GeometryWidgets
qmake -o Makefile geometryWidgets.pro
make ver=release

cd ../MainWidgets
qmake -o Makefile mainwidgets.pro
make ver=release

cd ../PluginManager
qmake -o Makefile PluginManager.pro
make ver=release

cd ../Gmsh
qmake -o Makefile GmshModule.pro
#cp -d ./gmsh441/lib/*    ../../output/bin/
make ver=release

cd ../UserGuidence
qmake -o Makefile UserGuidence.pro
make ver=release

cd ../mainWindow
qmake -o Makefile mainWindow.pro
make ver=release

cd ../main
qmake -o Makefile main.pro
make ver=release


cd ../PluginMeshDataExchange
qmake -o Makefile meshDataExchangePlugin.pro
make ver=release

cd ../PluginCustomizer
qmake -o Makefile pluginCustomizer.pro
make ver=release


