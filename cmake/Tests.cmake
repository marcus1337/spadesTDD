include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()
add_executable(unit_tests "test/testAPI.cpp" "test/testGlobal.cpp" "test/testDeck.cpp")
target_link_libraries(unit_tests GTest::gtest_main ${PROJECT_NAME})
gtest_discover_tests(unit_tests)