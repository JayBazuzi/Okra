@SETLOCAL
@ECHO OFF
@IF NOT EXIST .build mkdir .build
call :exec cd .build
call :exec cmake -Werror=dev -Werror=deprecated "%~dp0"
call :exec cmake --build . -- /verbosity:minimal
call :exec ctest --verbose
goto :EOF

    :exec
echo %~nx0: %*
%*
