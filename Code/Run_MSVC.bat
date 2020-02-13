@ECHO OFF
SET "D:\Qt_64\5.4\msvc2013_64_opengl\bin;%PATH%"

subwcrev.exe %~dp0 %~dp0\qrc\VersionNoTemplate.h %~dp0\qrc\VersionNo.h

@echo off
mode con lines=30 cols=60
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
cd /d "%~dp0"

REM *** To start build on MSVC90 ***
start "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.exe" FastCAE.sln

