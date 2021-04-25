@echo off

REM TODO  - can we just build both with one exe? 

subst w: C:\Users\Trost\Documents\0-CS\BrekoutGame
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
set path = C:\Users\Trost\Documents\0-CS\BrekoutGame;%path%