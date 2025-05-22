cmake_minimum_required(VERSION 3.14)
project(ArchPkgUI VERSION 1.0.0)

# Automatically generate version.h from version.h.in
set(APP_NAME "ArchPkgUI")

# Optional: auto-get version from git
execute_process(
    COMMAND git describe --tags --always
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(APP_VERSION ${GIT_VERSION})

# Create version header from template
configure_file(
    ${CMAKE_SOURCE_DIR}/version.h.in
    ${CMAKE_BINARY_DIR}/version.h
)

# Tell compiler where to find generated version.h
include_directories(${CMAKE_BINARY_DIR})

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base xrc html qa net xml)

include(${wxWidgets_USE_FILE})

# Grab all .cpp files in the current directory
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} alpm)
