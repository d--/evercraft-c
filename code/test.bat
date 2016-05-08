@echo off
if not exist ..\build (
    mkdir ..\build
)
pushd ..\build

if exist character_test.exe (
    del character_test.exe
)

clang ../code/character_test.c -o character_test.exe

character_test.exe

popd
