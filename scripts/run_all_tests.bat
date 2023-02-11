@echo off

set path="%~dp0..\build\output\debug\bin\"

set error=0

echo Running CPU40xx tests...
call %path%cpu40xx_tests.exe
if %errorlevel% neq 0 set error=%errorlevel%

echo Running CPU8080 tests...
call %path%cpu8080_tests.exe
if %errorlevel% neq 0 set error=%errorlevel%

echo Running gameboy tests...
call %path%gameboy_tests.exe
if %errorlevel% neq 0 set error=%errorlevel%

exit /B %error%
