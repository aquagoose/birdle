cmake_minimum_required(VERSION 3.25)
project(birdle)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE BIRDLE_SRC CONFIGURE_DEPENDS src/*.h src/*.cpp)
add_executable(birdle WIN32 ${BIRDLE_SRC})

add_subdirectory(lib/SDL)
target_link_libraries(birdle PRIVATE SDL3::SDL3)

add_subdirectory(lib/glm)
target_link_libraries(birdle PRIVATE glm::glm)