cmake_minimum_required(VERSION 3.29)

project(axp VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==> Add option to force ANSI-colors (useful for building with Ninja)
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)

if (${FORCE_COLORED_OUTPUT})
  if     ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
  endif ()
endif ()

add_library(axp STATIC src/exceptions.cpp src/strings.cpp src/source-location.cpp)

target_include_directories(axp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(axp PUBLIC fmt)
