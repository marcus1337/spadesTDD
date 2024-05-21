cmake_minimum_required(VERSION 3.10.0)
project(spades)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/Linux.cmake)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

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
"src/rules/Trick.cpp"
"src/rules/Deck.cpp"
"src/rules/ScoreSettings.cpp"

"src/data/Move.cpp"
"src/history/HistoryState.cpp"
"src/history/SpadesHistory.cpp"

"src/spades_ai/Analyze.cpp"
"src/spades_ai/AIBid.cpp"
"src/spades_ai/RandomAIPlacer.cpp"
"src/spades_ai/NetAIPlacer.cpp"
"src/spades_ai/DefaultPlacer.cpp"
"src/spades_ai/Observation.cpp"
"src/spades_ai/SuitSupplyObservation.cpp"
"src/spades_ai/AIController.cpp"

"src/spades_ai/neuralnet/Neuralnet.cpp" 
"src/spades_ai/neuralnet/WeightRandomizer.cpp" 
"src/spades_ai/neuralnet/NetworkCodec.cpp" 
"src/spades_ai/neuralnet/Layer.cpp" 
"src/spades_ai/neuralnet/Node.cpp"
"src/spades_ai/logic/NetUtils.cpp"
"src/spades_ai/logic/Action.cpp"
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "include/spades/pch.h")
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
    $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIR}>
)
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen)

cmake_policy(SET CMP0077 NEW)
option(BUILD_TESTING "Build the testing" ON)
if(BUILD_TESTING)
  include(cmake/Tests.cmake)
endif()

