@SETLOCAL
@ECHO OFF

@IF NOT EXIST .build mkdir .build
cd .build

call :exec cmake ..
if ERRORLEVEL 1 goto :EOF

call :exec cmake --build .
if ERRORLEVEL 1 goto :EOF

call :exec ctest
if ERRORLEVEL 1 goto :EOF

goto :EOF

    :exec
echo %~nx0: %*
%*
