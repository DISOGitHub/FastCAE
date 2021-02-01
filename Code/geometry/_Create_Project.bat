 

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 10.0.17763.0

SET "PATH=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\;C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64;%PATH%"  
 
 
qmake CONFIG+=X64 -tp vc geometry.pro 
 
pause 
