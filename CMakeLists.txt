cmake_minimum_required(VERSION 3.10)
project(hoshi-lang-stdlib)

file(GLOB ZIP_SRC lib/zip/*.c)
file(GLOB SQLITE_SRC lib/sqlite/*.c)

add_library(std STATIC ${ZIP_SRC} ${SQLITE_SRC})