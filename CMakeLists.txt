cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

project(
  c-algorithms
  VERSION 1.2.0
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "c-algorithms/src/*.[ch]")
add_library(c-algorithms ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "c-algorithms/src")

project(
  sds
  VERSION 2.0.0
  LANGUAGES C)
add_library(sds sds/sds.c)
target_include_directories(${PROJECT_NAME} PUBLIC "sds")

project(
  regexc
  VERSION 0.1.0
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "c/*.[ch]")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE c-algorithms)
target_link_libraries(${PROJECT_NAME} PRIVATE sds)

project(
  vec
  VERSION 0.2.1
  LANGUAGES C)
add_library(vec vec/src/vec.c)
target_include_directories(${PROJECT_NAME} PUBLIC vec/src)

project(
  regex-plainc
  VERSION 0.1.0
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "plainc/*.[ch]")
add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(cbitset)
target_link_libraries(${PROJECT_NAME} cbitset vec)

project(
  regex-cpp
  VERSION 0.1.0
  LANGUAGES CXX)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "cpp/*.[ch]pp")
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
