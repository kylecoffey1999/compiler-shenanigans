cmake_minimum_required(VERSION 3.18)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

project(
  c-algorithms
  VERSION 1.2.0
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "c-algorithms/src/*.[ch]")
add_library(c-algorithms ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC c-algorithms/src)

project(
  sds
  VERSION 2.0.0
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "sds/*.[ch]")
add_library(sds ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC sds)

project(
  vec
  VERSION 0.2.1
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "vec/src/*.[ch]")
add_library(vec ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC vec/src)

add_subdirectory(cbitset)

project(
  regexc
  VERSION 0.1.0
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "c/*.[ch]")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE c-algorithms)
target_link_libraries(${PROJECT_NAME} PRIVATE sds)

project(
  regex-plainc
  VERSION 0.1.0
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "plainc/*.[ch]")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} cbitset vec gc)

project(
  regex-cpp
  VERSION 0.1.0
  LANGUAGES CXX)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "cpp/*.[ch]pp")
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
