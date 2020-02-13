call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64\vcvars64.bat"

SET "PATH=C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin;%PATH%"

qmake CONFIG+=X64 -tp vc GmshModule.pro

mkdir ..\..\output\bin_d\gmsh
mkdir ..\..\output\bin\gmsh

copy /y ".\gmsh441\gmsh.exe" "..\..\output\bin_d\gmsh"
copy /y ".\gmsh441\gmsh.exe" "..\..\output\bin\gmsh"

copy /y ".\gmsh441\gmsh.Geo" "..\..\output\bin_d\gmsh"
copy /y ".\gmsh441\gmsh.Geo" "..\..\output\bin\gmsh"


pause
