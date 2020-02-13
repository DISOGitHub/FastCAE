SET "PATH=C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin;%PATH%"

lupdate -verbose CurveAnalyse.pro

lupdate -no-obsolete CurveAnalyse.pro

cd  translations/
lrelease CurveAnalyse_zh_CN.ts

pause
