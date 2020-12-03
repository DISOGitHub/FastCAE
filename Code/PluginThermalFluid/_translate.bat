SET "PATH=C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin;%PATH%"

lupdate -verbose ThermalFluid.pro


lupdate -no-obsolete ThermalFluid.pro


cd  ./resource/
lrelease TFCoupling_zh_CN.ts


pause
