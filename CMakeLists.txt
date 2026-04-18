cmake_minimum_required(VERSION 3.21)

project(WOHModCreator
    VERSION 0.2.0
    DESCRIPTION "Desktop editor for World of Horror mods"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

option(BUILD_TESTS "Build tests" ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

if(BUILD_TESTS)
    include(FetchContent)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(src/app)
add_subdirectory(src/core)
add_subdirectory(src/ito)

if (BUILD_TESTS) 
    enable_testing()
    add_subdirectory(tests)
endif()