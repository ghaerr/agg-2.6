cmake_minimum_required(VERSION 3.13...3.19)

project(antigrain
    VERSION 2.6
    DESCRIPTION "subpixel picture resolution algorithms"
    LANGUAGES CXX C
)

option(USE_GPC "use of gpc lib. caution! non-commertial licence only!" OFF)

# support conan package manager
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# support for clang tidy
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

if(WIN32)
    add_subdirectory(agg2d)
endif()
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(myapp)
