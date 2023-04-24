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

echo Running CPU_x80 tests...
call cpu_x80_tests.exe
if %errorlevel% neq 0 set error=%errorlevel%

echo Running CPU6502 tests...
call cpu6502_tests.exe
if %errorlevel% neq 0 set error=%errorlevel%

echo Running gameboy tests...
call gameboy_tests.exe
if %errorlevel% neq 0 set error=%errorlevel%

popd

if %error% equ 0 (echo All tests passed.) else (echo Some tests failed.)

exit /B %error%
