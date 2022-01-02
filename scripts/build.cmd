@echo off

set AND=if
set THEN=(
set ENDIF=)

set GENERATOR="%1"
set ARCHITECTURE="%2"
set BUILDTYPE="%3"

set GENERATOR_INTERNAL=""
if %GENERATOR% == "mvs16" set GENERATOR_INTERNAL="Visual Studio 16 2019"
if %GENERATOR_INTERNAL% == "" goto :error_param_generator

if NOT %ARCHITECTURE% == "Win32" %AND% NOT %ARCHITECTURE% == "x64" %THEN%
	goto :error_param_architecture
%ENDIF%
if NOT %BUILDTYPE% == "Debug" %AND% NOT %BUILDTYPE% == "Release" %THEN%
	goto :error_param_buildtype
%ENDIF%

set CMAKE_PARAMS_INTERNAL=-DCMAKE_CONFIGURATION_TYPES=%BUILDTYPE%

set BUILD_DIR=..\build\%GENERATOR%_%ARCHITECTURE%_%BUILDTYPE%
echo -- CMake build directory: %BUILD_DIR%
mkdir %BUILD_DIR% & pushd %BUILD_DIR%

set CMAKE_GENERATE_COMMAND=-G %GENERATOR_INTERNAL% %CMAKE_PARAMS_INTERNAL% -A %ARCHITECTURE% -S ..\..
echo -- CMake generate command: %CMAKE_GENERATE_COMMAND%
cmake %CMAKE_GENERATE_COMMAND%
if errorlevel 1 %THEN%
	goto :error_cmake_generate
%ENDIF%
echo -- Generating OK

REM set CMAKE_BUILD_COMMAND=--build . --config %BUILDTYPE% --target ALL_BUILD
REM echo -- CMake build command: %CMAKE_BUILD_COMMAND%
REM cmake %CMAKE_BUILD_COMMAND%
REM if errorlevel 1 %THEN%
	REM goto :error_cmake_build
REM %ENDIF%
REM echo -- Build OK

popd
goto :eof

:error_param_generator
echo ERROR: Incorrect generator was given (mvc16)
exit /B 1

:error_param_architecture
echo ERROR: Build architecture should be set (Win32, x64)
exit /B 2

:error_param_buildtype
echo ERROR: Build type must be specified (Debug, Release)
exit /B 3

:error_cmake_generate
echo ERROR: CMake generating NOK
popd
exit /B 4

:error_cmake_build
echo ERROR: CMake build NOK
popd
exit /B 5