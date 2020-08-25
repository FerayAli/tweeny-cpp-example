cmake_minimum_required(VERSION 3.5)

project(tweeny-cpp-example LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_VERSION 2.3)
set(SFML_COMPONENTS graphics window system)

find_package(SFML ${SFML_VERSION} COMPONENTS ${SFML_COMPONENTS} REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

add_subdirectory(tweeny-cpp)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
target_link_libraries(${PROJECT_NAME} tweeny)
