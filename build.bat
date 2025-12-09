@echo off
mkdir build
cd build
cmake .. -A x64 -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release
msbuild SuperBLT.sln /t:Build /p:Configuration=Release
cd ..