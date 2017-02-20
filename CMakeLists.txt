CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR}/ConsoleApplication1)

FILE(GLOB_RECURSE ALL_SOURCE_FILES 
    "ConsoleApplication1/*.cpp"
    )
FILE(WRITE ${CMAKE_BINARY_DIR}/okra_main.cpp "")
FILE(APPEND ${CMAKE_BINARY_DIR}/okra_main.cpp "#include \"stdafx.h\"\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/okra_main.cpp "#define OKRA_MAIN\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/okra_main.cpp "#include \"Okra.h\"\n")
ADD_EXECUTABLE(all_tests ${ALL_SOURCE_FILES} ${CMAKE_BINARY_DIR}/okra_main.cpp)

FOREACH(file ${ALL_SOURCE_FILES})
    file(RELATIVE_PATH executablefile ${CMAKE_CURRENT_SOURCE_DIR} ${file})
    STRING(REPLACE ".cpp" "" executablefile ${executablefile} )
    STRING(REPLACE "/" "__" executablefile ${executablefile} )
    STRING(REPLACE " " "_" executablefile ${executablefile} )
    ADD_EXECUTABLE(${executablefile} ${file})
    TARGET_COMPILE_DEFINITIONS(${executablefile} PRIVATE OKRA_MAIN=1)
ENDFOREACH(file)
