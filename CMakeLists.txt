cmake_minimum_required(VERSION 3.10.0)
project(spades)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/Linux.cmake)

add_library(${PROJECT_NAME} STATIC "src/Spades.cpp" 
"src/data/card/Card.cpp" 
"src/data/card/CardComparison.cpp"
"src/data/card/CardValue.cpp"
"src/data/card/CardValueSerialization.cpp"
"src/data/SpadesMemento.cpp"
"src/data/Score.cpp"
"src/data/State.cpp"
"src/data/PortableRandom.cpp"

"src/rules/BidVariation.cpp"
"src/rules/BidVariationController.cpp"
"src/rules/TrumpVariation.cpp"
"src/rules/TrumpVariationController.cpp"
"src/rules/Turn.cpp"
"src/rules/Trick.cpp"
"src/rules/Deck.cpp"

"src/history/SpadesHistory.cpp"
"src/history/SpadesCommand.cpp"
"src/history/SpadesCommandOrder.cpp"
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
)

option(BUILD_TESTING "Build the testing" ON)
if(BUILD_TESTING)
  include(cmake/Tests.cmake)
endif()

