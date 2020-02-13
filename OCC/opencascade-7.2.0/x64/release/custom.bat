echo off

rem include other custom.bat files with specific 3rdparty paths

if exist "%~dp0custom_vc12_64.bat" (
  call "%~dp0custom_vc12_64.bat" %1 %2 %3 
)

