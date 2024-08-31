cmake_minimum_required (VERSION 3.0)

set(CMAKE_C_COMPILER wllvm)
set(CMAKE_CXX_COMPILER wllvm++)

project (MyCoolExample)
set(CMAKE_PROJECT_NAME "MyCoolExample")

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MYCOOLEXAMPLE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
  ${MYCOOLEXAMPLE_SRC_DIR}/include
)

add_subdirectory(include)
add_subdirectory(lib)

add_executable(myprogram
  main.cpp
)

target_link_libraries(myprogram
  MyCoolLib_all
)
