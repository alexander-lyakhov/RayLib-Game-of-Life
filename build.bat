@echo off
cls

set "FILENAME=%~1"
if not defined FILENAME set "FILENAME=main.c"

SET "COMPILER= C:\raylib\w64devkit\bin\gcc.exe"

echo. && echo [46;97m COMPILING... [0m && echo. && %COMPILER% %FILENAME% -Os -s -o main.exe ^
	-IC:\raylib\w64devkit\include ^
	-LC:\raylib\w64devkit\lib ^
	-lraylib ^
	-lopengl32 ^
	-lgdi32 ^
	-lwinmm && if %ERRORLEVEL% EQU 0 main.exe
