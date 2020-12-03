SET "PATH=C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin;%PATH%"

lupdate -verbose HullCutPlugin.pro

lupdate -no-obsolete HullCutPlugin.pro

cd  resource/translation/
lrelease transtrations_zh_CN.ts

pause
