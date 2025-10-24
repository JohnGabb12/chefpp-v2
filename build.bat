@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl.exe /EHsc /std:c++17 /Fe:%1\build\chefpp.exe /Fo:%1\build\ %1\main.cpp /I%1
