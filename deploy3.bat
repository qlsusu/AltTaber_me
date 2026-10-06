@echo off
set QT_DIR=C:\Qt\6.12.0\msvc2022_64
set DEST=D:\tool\AltTaber_me\build\Release

copy "%QT_DIR%\plugins\styles\qmodernwindowsstyle.dll" "%DEST%\styles\"

echo Done
