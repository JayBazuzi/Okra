@SETLOCAL
@ECHO OFF

@IF NOT EXIST .build mkdir .build
cd .build

call :exec cmake ..
if ERRORLEVEL 1 goto :EOF

call :exec cmake --build .
if ERRORLEVEL 1 goto :EOF

set CTEST_OUTPUT_ON_FAILURE=1
call :exec ctest --parallel 4
if ERRORLEVEL 1 goto :EOF

goto :EOF

    :exec
echo %~nx0: %*
%*
