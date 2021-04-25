@echo off
pushd ..\build
cl -nologo -Zi -FC ..\code\win32_platform.c /link user32.lib gdi32.lib -incremental:no -opt:ref

popd