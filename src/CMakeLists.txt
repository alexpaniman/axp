add_executable(axp-test main.cpp)

target_include_directories(axp-test
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(axp-test axp)
