cmake_minimum_required(VERSION 3.28)
project(UniversalCppProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

file(GLOB_RECURSE ALL_SRCS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

file(GLOB_RECURSE ALL_MODULES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cppm"
)

add_executable(${ONAME} ${ALL_SRCS})

target_sources(${ONAME}
    PUBLIC
    FILE_SET CXX_MODULES
    FILES ${ALL_MODULES}
)

target_link_libraries(${ONAME} PRIVATE sfml-graphics sfml-window sfml-system)

target_include_directories(${ONAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
