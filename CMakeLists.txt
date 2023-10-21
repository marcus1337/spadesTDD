cmake_minimum_required(VERSION 3.10.0)
project(spades)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0135 NEW)

add_library(${PROJECT_NAME} STATIC "src/Spades.cpp" "include/data/Card.h" "include/Deck.h" "include/global/PortableRandom.h")
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
)

if(BUILD_TESTING)
  include(cmake/Tests.cmake)
endif()

