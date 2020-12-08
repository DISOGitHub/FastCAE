SET "PATH=C:\Qt\Qt5.4.2\5.4\msvc2013_64_opengl\bin;%PATH%"

lupdate -verbose pluginCustomizer.pro

lupdate -no-obsolete pluginCustomizer.pro

cd  resource/translation/
lrelease transtrations_zh_CN.ts

pause
