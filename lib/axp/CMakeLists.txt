add_library(axp STATIC null.cpp)

target_include_directories(axp PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})
