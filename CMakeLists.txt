cmake_minimum_required(VERSION 4.0)
project(birdle)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Build)

file(GLOB_RECURSE BIRDLE_SRC CONFIGURE_DEPENDS src/*.h src/*.cpp)
file(GLOB_RECURSE BIRDLE_MODULES CONFIGURE_DEPENDS src/*.cppm)
add_executable(birdle WIN32 ${BIRDLE_SRC})
target_sources(birdle PRIVATE FILE_SET CXX_MODULES FILES ${BIRDLE_MODULES})

add_subdirectory(lib/SDL)
target_link_libraries(birdle PRIVATE SDL3::SDL3)

add_subdirectory(lib/glm)
target_link_libraries(birdle PRIVATE glm::glm)

add_subdirectory(lib/glad)
target_link_libraries(birdle PRIVATE glad)

add_subdirectory(lib/stb)
target_link_libraries(birdle PRIVATE stb)

add_subdirectory(lib/cpptrace)
target_link_libraries(birdle PRIVATE cpptrace::cpptrace)

add_custom_command(
        TARGET birdle
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Content ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Content
        VERBATIM POST_BUILD)