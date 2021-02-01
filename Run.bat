@echo off
echo  --------------------------------------------------------------
echo  FastCAE Development environment Configuration For Windows10
echo.
echo      This procedure will complete the following work:
echo      1. Confirm the installation of essential development tools;
echo      2. Copy third party libraries;
echo      3. Generate essential Debug environment;
echo      4. Open Visual Studio IDE.
echo.
echo   Please make sure that QT and Visual Studio are installed correctly in the current operating system!
echo.
echo   In case of installation problems, please contact FastCAE development team!
echo.
echo  -----------------------------------------------------------------
echo.  
echo  Please make sure that QT 5.14.2 and Visual Studio 2017 are installed correctly! 
echo.

cd /d "%~dp0"

set currentPath=%cd%

echo Please input the path of qmake.exe, For example: C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\
echo.
set /p qmakeDir=Waitting for input: 
echo.
set qmakePath=%qmakeDir%\qmake.exe
echo.


echo Please input the path of Visual Studio 2017, For example: C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\
echo (There are VC, common7 and other folders in this directory)
echo.
set /p VSDir=Waitting for input: 
echo.
echo Please input Version of Windows SDK, For example: 10.0.17763.0
echo.
set /p WSDK=Waitting for input: 
echo.
echo Generating batch script...
echo.

:Replace
set codePath=%currentPath%\Code
for /d  %%d in ("%currentPath%\Code\*") do (
if exist "%%d\_Create_project.bat" (
cd %%d
for /f "delims=" %%a in ('dir /b *.pro') do (
rem 各工程的qmake脚本
echo. > %%d\_Create_project.bat
echo call "%VSDir%\VC\Auxiliary\Build\vcvarsall.bat"  x64 %WSDK%>> %%d\_Create_project.bat

echo. >> %%d\_Create_project.bat
echo SET "PATH=%qmakeDir%;%VSDir%\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64;%%PATH%%" >> %%d\_Create_project.bat 
echo. >> %%d\_Create_project.bat
echo qmake CONFIG+=X64 -tp vc %%a >> %%d\_Create_project.bat
echo. >> %%d\_Create_project.bat
echo pause >> %%d\_Create_project.bat
)
)
)

rem 总体的工程脚本
echo. >%codePath%\Create_X64_Project.bat
echo call "%VSDir%\VC\Auxiliary\Build\vcvarsall.bat"  x64 %WSDK%>>%codePath%\Create_X64_Project.bat 
echo. >> %codePath%\Create_X64_Project.bat 
echo SET "PATH=%qmakeDir%;%VSDir%\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64;%%PATH%%" >> %codePath%\Create_X64_Project.bat 
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
echo cd %codePath% >> %codePath%\Create_X64_Project.bat
echo qmake CONFIG+=X64 -tp vc FastCAE.pro >> %codePath%\Create_X64_Project.bat 
echo. >> %codePath%\Create_X64_Project.bat 
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

echo Batch script information update completed!
echo.
echo The next step is to generate the development environment...
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
copy /y "%codePath%\Gmsh\gmsh454\gmsh.exe" "%currentPath%\output\bin_d\gmsh"
copy /y "%codePath%\Gmsh\gmsh454\gmsh.exe" "%currentPath%\output\bin\gmsh"
copy /y "%codePath%\Gmsh\gmsh454\gmsh.Geo" "%currentPath%\output\bin_d\gmsh"
copy /y "%codePath%\Gmsh\gmsh454\gmsh.Geo" "%currentPath%\output\bin\gmsh"

echo Visual studio 2017 will be launched next!
echo.
pause
call "%codePath%\Run_MSVC.bat"


:finish
echo The setup project is completed, and the program will exit soon!
echo.
pause
exit
