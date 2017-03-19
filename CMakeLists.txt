CMAKE_MINIMUM_REQUIRED(VERSION 3.2.2)

set( CMAKE_CXX_STANDARD 14 )
enable_testing ()
include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB_RECURSE ALL_SOURCE_FILES 
    "Tests/*.cpp"
    )

FOREACH(file ${ALL_SOURCE_FILES})
    FILE(RELATIVE_PATH executablefile ${CMAKE_CURRENT_SOURCE_DIR}/Tests ${file})
    # https://cmake.org/cmake/help/v3.0/policy/CMP0037.html
    STRING(REPLACE ".cpp" "" executablefile ${executablefile} )
    STRING(REPLACE "/" "__" executablefile ${executablefile} )
    STRING(REGEX REPLACE "[^A-Za-z0-9_.+-]" "_" executablefile ${executablefile} )
    ADD_EXECUTABLE(${executablefile} ${file})
    ADD_TEST(${executablefile} ${executablefile})
    TARGET_COMPILE_DEFINITIONS(${executablefile} PRIVATE OKRA_MAIN=1)
    IF (WIN32)
    ELSE()
        target_link_libraries(${executablefile} stdc++fs )
    ENDIF()
ENDFOREACH(file)
