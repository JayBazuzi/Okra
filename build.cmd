@SETLOCAL
@IF NOT EXIST .build mkdir .build
cd .build
cmake -Werror=dev -Werror=deprecated %~dp0
cmake --build . -- /verbosity:minimal
ctest
