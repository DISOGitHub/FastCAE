SET "PATH=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin;%PATH%"

lupdate -verbose pluginCustomizer.pro

lupdate -no-obsolete pluginCustomizer.pro

cd  resource/translation/
lrelease transtrations_zh_CN.ts

pause
