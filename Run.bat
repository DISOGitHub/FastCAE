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
echo %currentPath%

echo 请输入qmake.exe所在路径，例如：C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin\：
echo.
set /p qmakeDir=请输入:
echo.
set qmakePath=%qmakeDir%\qmake.exe
echo %qmakePath%


echo 请输入Visual Studio 2013安装目录，例如：C:\Program Files (x86)\Microsoft Visual Studio 12.0\：
echo (该目录下存在VC、Common7等文件夹)
echo.
set /p VSDir=请输入:
echo.
echo "%VSDir%"

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
echo 接下来将生成开发环境。。。
pause

cd %codePath%

call "%codePath%\Copy_DLLs.bat"
call "%codePath%\Copy_Pys.bat"
call "%codePath%\Create_X64_Project.bat"

echo 下面将启动Visual Studio 2013
pause
call "%codePath%\Run_MSVC.bat"


:finish
echo 设置项目全部完成，程序即将退出！
echo.
pause
exit
