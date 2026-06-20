cmake_minimum_required(VERSION 4.0)
project(glad LANGUAGES C)

add_library(glad STATIC
        include/glad/gl.h
        include/KHR/khrplatform.h
        src/gl.c
)

target_include_directories(glad PUBLIC include/)