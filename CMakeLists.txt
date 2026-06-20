cmake_minimum_required(VERSION 4.0)
project(birdle)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE BIRDLE_SRC CONFIGURE_DEPENDS
        src/*.h
        src/*.cpp
)

add_executable(birdle ${BIRDLE_SRC})

add_subdirectory(external/SDL)
target_link_libraries(birdle PRIVATE SDL3::SDL3)

add_subdirectory(external/glad)
target_link_libraries(birdle PRIVATE glad)

add_subdirectory(external/cpptrace)
target_link_libraries(birdle PRIVATE cpptrace::cpptrace)