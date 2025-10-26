cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(EXCLUDE_COMPONENTS esp_lcd)
# set(CMAKE_C_FLAGS "-Wignored-qualifiers")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(GB)
