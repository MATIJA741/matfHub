 
cmake_minimum_required(VERSION 3.22)
project(Test VERSION 1.0.0)

# add_library(
#     plotter
#     src/plotter.hpp
#     plotter.cpp
# )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

set(CMAKE_CXX_FLAGS "-O0 -coverage")

include(FetchContent)
FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master) # or whatever tag you want

FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# find required tools
find_program(LCOV lcov REQUIRED)
find_program(GENHTML genhtml REQUIRED)

# add coverage target
add_custom_target(coverage
  # gather data
  COMMAND ${LCOV} --directory . --capture --output-file coverage.info
  # generate report
  COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})


add_executable(test_stat statistics.test.cpp ../src/statistics.cpp )
target_link_libraries(test_stat PUBLIC matplot)

add_executable(test_plot plotter.test.cpp ../src/plotter.cpp)
target_link_libraries(test_plot PUBLIC matplot)


