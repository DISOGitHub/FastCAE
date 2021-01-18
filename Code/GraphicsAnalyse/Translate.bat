SET "PATH=C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin;%PATH%"

lupdate -verbose GraphicsAnalyse.pro

lupdate -no-obsolete GraphicsAnalyse.pro

cd  translations/
lrelease GraphicsAnalyse_zh_CN.ts

pause
