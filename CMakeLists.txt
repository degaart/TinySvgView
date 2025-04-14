cmake_minimum_required(VERSION 3.20)
project(TinySvgView)

cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
include(FetchContent)
FetchContent_Declare(
    fmt
    URL https://github.com/fmtlib/fmt/releases/download/11.1.4/fmt-11.1.4.zip
    URL_HASH SHA256=49b039601196e1a765e81c5c9a05a61ed3d33f23b3961323d7322e4fe213d3e6
)

FetchContent_Declare(
    plutosvg
    GIT_REPOSITORY https://github.com/sammycage/plutosvg.git
    GIT_TAG v0.0.6
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(plutosvg)
add_subdirectory(src)

