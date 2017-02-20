cmake_minimum_required(VERSION 3.7)
file(GLOB_RECURSE ALL_SOURCE_FILES 
    "ConsoleApplication1/*.cpp"
    )
add_executable(Project1 ${ALL_SOURCE_FILES})
