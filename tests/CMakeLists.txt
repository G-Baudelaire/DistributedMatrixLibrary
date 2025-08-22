cmake_minimum_required(VERSION 4.0)

# GoogleTest requires at least C++17
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/6986c2b575f77135401a4e1c65a7a42f20e18fef.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(DMLibTests
        MatrixTest.cc
        AdditionTest.cpp
)
target_link_libraries(DMLibTests
        PRIVATE
        DMLib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(DMLibTests)
