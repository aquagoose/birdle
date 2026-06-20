cmake_minimum_required(VERSION 4.2)
project(birdle)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE BIRDLE_SRC CONFIGURE_DEPENDS
        src/*.h
        src/*.cpp
)

add_executable(birdle ${BIRDLE_SRC})

add_subdirectory(external/SDL)
target_link_libraries(birdle PRIVATE SDL3::SDL3)