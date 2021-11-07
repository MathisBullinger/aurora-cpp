cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

project(aur_test LANGUAGES CXX)

SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}/../src")

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_FILES ../src/**/*.test.cpp)

add_executable(${PROJECT_NAME} ${TEST_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main gmock_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
