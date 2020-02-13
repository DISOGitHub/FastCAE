echo off

rem CASDEB comes as third argument

if /I "%VCVER%" == "vc12" (
  if "%ARCH%" == "64" (
    rem set environment variables used by OCCT
    set CSF_FPE=0

    set "TCL_DIR=%THIRDPARTY_DIR%/tcltk-86-64/bin"
    set "TK_DIR=%THIRDPARTY_DIR%/tcltk-86-64/bin"
    set "FREETYPE_DIR=%THIRDPARTY_DIR%/freetype-2.5.5-vc12-64/bin"
    set "FREEIMAGE_DIR=%THIRDPARTY_DIR%/freeimage-3.17.0-vc12-64/bin;"
    set "EGL_DIR="
    set "GLES2_DIR="
    set "GL2PS_DIR=%THIRDPARTY_DIR%/gl2ps-1.3.8-vc12-64/bin;"
    set "TBB_DIR="
    set "VTK_DIR=D:/XMG/Library/VTK/vtk7.7.1/vtk_7.7.1_x64_Release/bin"
    set "FFMPEG_DIR=%THIRDPARTY_DIR%/ffmpeg-3.3-64/bin"

    if not "" == "" (
      set "QTDIR="
    )
    set "TCL_VERSION_WITH_DOT=8.6"
    set "TK_VERSION_WITH_DOT=8.6"

    set "CSF_OCCTBinPath=%CASROOT%/win64/vc12/bin%3"
    set "CSF_OCCTLibPath=%CASROOT%/win64/vc12/lib%3"

    set "CSF_OCCTIncludePath=%CASROOT%/inc"
    set "CSF_OCCTResourcePath=%CASROOT%/src"
    set "CSF_OCCTDataPath=%CASROOT%/data"
    set "CSF_OCCTSamplesPath=%CASROOT%/samples"
    set "CSF_OCCTTestsPath=%CASROOT%/tests"
    set "CSF_OCCTDocPath=%CASROOT%/doc"
  )
)

