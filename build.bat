@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cmake -S . -B build -G "Visual Studio 18 2026" -DQt6_DIR="C:/Qt/6.12.0/msvc2022_64/lib/cmake/Qt6" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-I C:/Qt/6.12.0/msvc2022_64/include -I C:/Qt/6.12.0/msvc2022_64/include/QtCore/6.12.0"
