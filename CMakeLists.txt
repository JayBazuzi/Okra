cmake_minimum_required(VERSION 3.7)
file(GLOB_RECURSE ALL_SOURCE_FILES 
    "ConsoleApplication1/format path for display.cpp" 
    "ConsoleApplication1/get common root of file paths.cpp"    
    )
add_executable(Project1 ${ALL_SOURCE_FILES})
