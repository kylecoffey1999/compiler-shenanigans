cmake_minimum_required(VERSION 3.21)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

project(
  regexc
  VERSION 0.1.0
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "c/*.[ch]")
add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(c-algorithms)
target_link_libraries(${PROJECT_NAME} PRIVATE "C-Algorithms")
target_include_directories(${PROJECT_NAME} PRIVATE c-algorithms/src)

add_subdirectory(sds)
target_link_libraries(${PROJECT_NAME} PRIVATE sds)

project(
  regex-plainc
  VERSION 0.1.0
  LANGUAGES C)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "plainc/*.[ch]")
add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(cbitset)
add_subdirectory(vec)
target_link_libraries(${PROJECT_NAME} cbitset vec)
target_include_directories(${PROJECT_NAME} PRIVATE vec/src)

project(
  regex-cpp
  VERSION 0.1.0
  LANGUAGES CXX)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "cpp/*.[ch]pp")
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
