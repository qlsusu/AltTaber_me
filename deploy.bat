@echo off
set QT_DIR=C:\Qt\6.12.0\msvc2022_64\bin
set DEST=D:\tool\AltTaber_me\build\Release

copy "%QT_DIR%\Qt6Core.dll" "%DEST%\"
copy "%QT_DIR%\Qt6Gui.dll" "%DEST%\"
copy "%QT_DIR%\Qt6Widgets.dll" "%DEST%\"
copy "%QT_DIR%\Qt6Xml.dll" "%DEST%\"
copy "%QT_DIR%\Qt6Network.dll" "%DEST%\"

echo Done
