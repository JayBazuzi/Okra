@SETLOCAL
@ECHO OFF

WHERE cmake > nul: 2>&1
IF ERRORLEVEL 1 (
    echo ERROR! CMake not found. Install from https://cmake.org/download/
    exit /b 1
)

IF NOT EXIST .build mkdir .build
cd .build

CALL :exec cmake .. -Wdev -Werror=dev -Wdeprecated -Werror=deprecated -G "Visual Studio 15 2017"
IF ERRORLEVEL 1 GOTO :EOF

CALL :exec cmake --build . -- /verbosity:minimal /nologo
IF ERRORLEVEL 1 GOTO :EOF

set CTEST_OUTPUT_ON_FAILURE=1
CALL :exec ctest --parallel 4
IF ERRORLEVEL 1 GOTO :EOF

GOTO :EOF

    :exec
echo.
echo %~nx0: %*
%*
