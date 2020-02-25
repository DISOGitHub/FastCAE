@echo off
echo  ——————————————————————————————————————————
echo    FastCAE开发环境一键配置 Windows10版
echo.
echo      本程序将完成以下几点工作：
echo      1. 确认安装必要的开发工具；
echo      2. 拷贝第三方库；
echo      3. 生成必要的运行环境；
echo      4. 打开Visual Studio IDE。
echo.
echo   请务必确认当前操作系统中 Qt, Visual Studio均已正确安装！
echo.
echo   如遇安装问题，请联系FastCAE开发小组！
echo.
echo  ———————————————————————————————————————————

echo.  
echo 请确认已经安装Qt5.4.2和Visual Studio 2013!
echo.

cd /d "%~dp0"

set currentPath=%cd%

echo 请输入qmake.exe所在路径，例如：C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin\
echo.
set /p qmakeDir=请输入:
echo.
set qmakePath=%qmakeDir%\qmake.exe
echo.


echo 请输入Visual Studio 2013安装目录，例如：C:\Program Files (x86)\Microsoft Visual Studio 12.0\
echo (该目录下存在VC、Common7等文件夹)
echo.
set /p VSDir=请输入:
echo.
echo 正在生成批处理脚本...
echo.

:Replace
set codePath=%currentPath%\Code
for /d  %%d in ("%currentPath%\Code\*") do (
if exist "%%d\_Create_project.bat" (
cd %%d
for /f "delims=" %%a in ('dir /b *.pro') do (
rem 各工程的qmake脚本
echo. > %%d\_Create_project.bat
echo call "%VSDir%\VC\bin\amd64\vcvars64.bat" >> %%d\_Create_project.bat

echo. >> %%d\_Create_project.bat
echo SET "PATH=%qmakeDir%;%%PATH%%" >> %%d\_Create_project.bat 
echo. >> %%d\_Create_project.bat
echo qmake CONFIG+=X64 -tp vc %%a >> %%d\_Create_project.bat
echo. >> %%d\_Create_project.bat
echo pause >> %%d\_Create_project.bat
)
)
)

rem 总体的工程脚本
echo. >%codePath%\Create_X64_Project.bat
echo call "%VSDir%\VC\bin\amd64\vcvars64.bat" >> %codePath%\Create_X64_Project.bat
echo. >> %codePath%\Create_X64_Project.bat 
echo SET "PATH=%qmakeDir%;%%PATH%%" >> %codePath%\Create_X64_Project.bat 
echo. >> %codePath%\Create_X64_Project.bat 
for /d  %%d in ("%currentPath%\Code\*") do (
if exist "%%d\_Create_project.bat" (
for /f "delims=" %%a in ('dir /b %%d\*.pro') do (
echo cd %%d >> %codePath%\Create_X64_Project.bat
echo qmake CONFIG+=X64 -tp vc %%a >> %codePath%\Create_X64_Project.bat
echo. >> %codePath%\Create_X64_Project.bat 
)
)
)
echo pause >> %codePath%\Create_X64_Project.bat 

:VS
rem 启动Visual studio 2013
echo. > %codePath%\Run_MSVC.bat
echo @echo off>> %codePath%\Run_MSVC.bat
echo %%1 mshta vbscript:CreateObject^("Shell.Application"^).ShellExecute^("cmd.exe","/c %%~s0 ::","","runas",1^)^(window.close^)^&^&exit >> %codePath%\Run_MSVC.bat
echo cd /d "%%~dp0">> %codePath%\Run_MSVC.bat
echo. >> %codePath%\Run_MSVC.bat
echo start "%VSDir%\Common7\IDE\devenv.exe" FastCAE.sln >> %codePath%\Run_MSVC.bat
echo. >> %codePath%\Run_MSVC.bat

echo 批处理脚本信息更新完毕！
echo.
echo 接下来将生成开发环境。。。
echo.
pause

cd %codePath%
call "%codePath%\Copy_DLLs.bat"
cd %codePath%
call "%codePath%\Copy_Pys.bat"
cd %codePath%
call "%codePath%\Create_X64_Project.bat"

cd %currentPath%
if not exist "%qmakeDir%\Qt5Widgetsd.dll" (
copy /y "%qmakeDir%\Qt5Widgetsd.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Guid.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Cored.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Xmld.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5PrintSupportd.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5OpenGLd.dll" ".\output\bin_d"

copy /y "%qmakeDir%\Qt5WebKitWidgetsd.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Networkd.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5WebKitd.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Sensorsd.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5MultimediaWidgetsd.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Multimediad.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Positioningd.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5WebChanneld.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5PrintSupport.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Quickd.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Qmld.dll" ".\output\bin_d"
copy /y "%qmakeDir%\Qt5Sqld.dll" ".\output\bin_d"

copy /y "%qmakeDir%\icuin53.dll" ".\output\bin_d"
copy /y "%qmakeDir%\icuuc53.dll" ".\output\bin_d"
copy /y "%qmakeDir%\icudt53.dll" ".\output\bin_d"
cd .\output\bin_d\
mkdir .\platforms\
copy /y "%qmakeDir%\..\plugins\platforms\" ".\platforms\"
)
cd %currentPath%
if not exist "%qmakeDir%\Qt5Widgets.dll" (
copy /y "%qmakeDir%\Qt5Widgets.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Gui.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Core.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Xml.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5OpenGL.dll" ".\output\bin"

copy /y "%qmakeDir%\Qt5WebKitWidgets.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Network.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5WebKit.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Sensors.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5MultimediaWidgets.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Multimedia.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Positioning.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5WebChannel.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5PrintSupport.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Quick.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Qml.dll" ".\output\bin"
copy /y "%qmakeDir%\Qt5Sql.dll" ".\output\bin"
copy /y "%qmakeDir%\icuin53.dll" ".\output\bin"
copy /y "%qmakeDir%\icuuc53.dll" ".\output\bin"
copy /y "%qmakeDir%\icudt53.dll" ".\output\bin"
cd .\output\bin\
mkdir .\platforms\
copy /y "%qmakeDir%\..\plugins\platforms\" ".\platforms\"
)

cd %currentPath%

mkdir %currentPath%\output\bin_d\gmsh
mkdir %currentPath%\output\bin\gmsh
copy /y "%codePath%\Gmsh\gmsh441\gmsh.exe" "%currentPath%\output\bin_d\gmsh"
copy /y "%codePath%\Gmsh\gmsh441\gmsh.exe" "%currentPath%\output\bin\gmsh"
copy /y "%codePath%\Gmsh\gmsh441\gmsh.Geo" "%currentPath%\output\bin_d\gmsh"
copy /y "%codePath%\Gmsh\gmsh441\gmsh.Geo" "%currentPath%\output\bin\gmsh"

echo 下面将启动Visual Studio 2013
echo.
pause
call "%codePath%\Run_MSVC.bat"


:finish
echo 设置项目全部完成，程序即将退出！
echo.
pause
exit
