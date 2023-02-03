@echo off

set path="%~dp0..\build\output\debug\bin\"

echo Running CPU40xx tests...
call %path%cpu40xx_tests.exe

echo Running CPU8080 tests...
call %path%cpu8080_tests.exe
