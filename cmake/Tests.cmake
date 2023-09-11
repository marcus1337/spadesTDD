include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()
add_executable(cpp_test0 "test/test0.cpp")
target_link_libraries(cpp_test0 GTest::gtest_main ${PROJECT_NAME})
gtest_discover_tests(cpp_test0)