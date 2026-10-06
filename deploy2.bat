@echo off
set QT_DIR=C:\Qt\6.12.0\msvc2022_64
set DEST=D:\tool\AltTaber_me\build\Release

mkdir "%DEST%\platforms" 2>nul
copy "%QT_DIR%\plugins\platforms\qwindows.dll" "%DEST%\platforms\"

mkdir "%DEST%\styles" 2>nul
copy "%QT_DIR%\plugins\styles\qwindowsvistastyle.dll" "%DEST%\styles\"

echo Done
