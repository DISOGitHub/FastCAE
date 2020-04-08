SET "PATH=C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin;%PATH%"

lupdate -verbose mainWindow/mainWindow.pro
lupdate -verbose MainWidgets/mainwidgets.pro
lupdate -verbose settings/setting.pro
lupdate -verbose settings/setting.pro
lupdate -verbose moduleBase/moduleBase.pro
lupdate -verbose ProjectTree/ProjectTree.pro
lupdate -verbose PostWidgets/PostWidgets.pro
lupdate -verbose Material/Material.pro
lupdate -verbose IO/IO.pro
lupdate -verbose GeometryWidgets/geometryWidgets.pro
lupdate -verbose PluginManager/PluginManager.pro
lupdate -verbose Gmsh/GmshModule.pro
lupdate -verbose SolverControl/solverControl.pro
lupdate -verbose UserGuidence/UserGuidence.pro

lupdate -no-obsolete mainWindow/mainWindow.pro
lupdate -no-obsolete MainWidgets/mainwidgets.pro
lupdate -no-obsolete settings/setting.pro
lupdate -no-obsolete moduleBase/moduleBase.pro
lupdate -no-obsolete ProjectTree/ProjectTree.pro
lupdate -no-obsolete PostWidgets/PostWidgets.pro
lupdate -no-obsolete Material/Material.pro
lupdate -no-obsolete IO/IO.pro
lupdate -no-obsolete GeometryWidgets/geometryWidgets.pro
lupdate -no-obsolete PluginManager/PluginManager.pro
lupdate -no-obsolete Gmsh/GmshModule.pro
lupdate -no-obsolete SolverControl/solverControl.pro
lupdate -no-obsolete UserGuidence/UserGuidence.pro

cd  qrc/translations/
lrelease MainWindow_zh_CN.ts
lrelease MainWidgets_zh_CN.ts
lrelease Setting_zh_CN.ts
lrelease ModuleBase_zh_CN.ts
lrelease ProjectTree_zh_CN.ts
lrelease PostWidgets_zh_CN.ts
lrelease Material_zh_CN.ts
lrelease IO_zh_CN.ts
lrelease GeometryWidgets_zh_CN.ts
lrelease PluginManager_zh_CN.ts
lrelease GmshModule_zh_CN.ts
lrelease SolverControl_Zh_CN.ts
lrelease UserGuidence_zh_CN.ts

pause
