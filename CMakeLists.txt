CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(Okra)

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
ENDFOREACH(file)

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
ELSEIF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -WX")
ENDIF()
