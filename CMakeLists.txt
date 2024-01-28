cmake_minimum_required(VERSION 3.5)

project(matfHub VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB PROJECT_SOURCES
    "matfHub/src/*.cpp"
    "matfHub/include/*.hpp"
    "matfHub/include/*.h"
        "matfHub/*.ui"
        "matfHub/dependencies/*.cpp"
        "matfHub/dependencies/*.h"
)

##fetching necessary libraries
#network core
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Core)

#armadillo
find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})

#matplotplusplus
include(FetchContent)
FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master)

FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

##
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(matfHub
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        matfHub/include/notes.h
        matfHub/src/notes.cpp
        matfHub/resources.qrc
        matfHub/src/schedulegenerator.cpp
        matfHub/include/schedulegenerator.h
        matfHub/include/helpers.h
        matfHub/include/calendar.h
        matfHub/src/calendar.cpp
        matfHub/include/config.hpp
        matfHub/include/views.h
        matfHub/src/config.cpp
        matfHub/src/views.cpp
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET matfHub APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(matfHub SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(matfHub
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Installing dependencies
set(MY_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/prebuild.sh")

add_custom_target(install_dependencies
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/prebuild.sh"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Installing dependencies and setting up directories"
)

#linking libraries
target_link_libraries(
    matfHub PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Core
    PUBLIC Matplot++::matplot ${ARMADILLO_LIBRARIES}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.matfHub)
endif()
set_target_properties(matfHub PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS matfHub
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(matfHub)
endif()

add_dependencies(matfHub install_dependencies)
