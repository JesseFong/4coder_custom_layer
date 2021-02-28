@echo off

pushd ..\..\
call custom\bin\buildsuper_x64-win.bat custom\4jesse_custom\4jesse_custom_main.cpp release
popd