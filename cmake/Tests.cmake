
find_package(GTest QUIET)
if(NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)
enable_testing()

add_executable(unit_tests 
"test/testAPI.cpp" 
"test/testRandom.cpp" 
"test/testDeck.cpp" 
"test/testScore.cpp" 
"test/testSerialization.cpp"
"test/testTrumpVariation.cpp"
"test/testBidVariation.cpp"
"test/testUndoRedo.cpp"
"test/testStateValidity.cpp"

"test/testAnalyze.cpp"
"test/testAIBid.cpp"
"test/testTrickWin.cpp"
)

target_precompile_headers(unit_tests PRIVATE "test/test_pch.h")
target_link_libraries(unit_tests GTest::gtest_main ${PROJECT_NAME})
gtest_discover_tests(unit_tests)