cmake_minimum_required(VERSION 3.9)
project(codegroscapasse C)

set(CMAKE_C_STANDARD 11)

set(PROJECT_HEADERS
        dictionary.h
)
set(PROJECT_SOURCES
        main.c
        dictionary.c
)

add_executable(codegroscapasse ${PROJECT_SOURCES} ${PROJECT_HEADERS})