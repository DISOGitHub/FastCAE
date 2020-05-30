 
@echo off
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit 
cd /d "%~dp0"
 
start "C:\Program Files (x86)\Microsoft Visual Studio 12.0\\Common7\IDE\devenv.exe" FastCAE.sln 
 
