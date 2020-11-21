# Axel '0vercl0k' Souchet - November 20 2020
cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
include(ExternalProject)

project(tests)

ExternalProject_Add(
  fmt
  GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
  GIT_TAG "7.1.2"
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/third_party/fmt"
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/third_party/fmt/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../include)

add_executable(
    tests
    tests.cc
)

add_compile_definitions(FMT_HEADER_ONLY)
add_dependencies(tests fmt)