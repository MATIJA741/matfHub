 
cmake_minimum_required(VERSION 3.16.3)
project(Test VERSION 1.0.0)

# add_library(
#     plotter
#     src/plotter.hpp
#     plotter.cpp
# )


include(FetchContent)
FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master) # or whatever tag you want

FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


add_executable(test_stat statistics.test.cpp ../src/statistics.cpp )
target_link_libraries(test_stat PUBLIC matplot)

add_executable(test_plot plotter.test.cpp ../src/plotter.cpp)
target_link_libraries(test_plot PUBLIC matplot)


