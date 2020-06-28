@echo off
pushd ..\
call vendor\binary\premake\premake5.exe vs2019
popd
PAUSE