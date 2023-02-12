@echo off

set path="%~dp0..\build\output\debug\bin\"

set error=0

pushd %path%

echo Running CPU40xx tests...
call cpu40xx_tests.exe
if %errorlevel% neq 0 set error=%errorlevel%

echo Running CPU8080 tests...
call cpu8080_tests.exe
if %errorlevel% neq 0 set error=%errorlevel%

echo Running gameboy tests...
call gameboy_tests.exe
if %errorlevel% neq 0 set error=%errorlevel%

popd

exit /B %error%
