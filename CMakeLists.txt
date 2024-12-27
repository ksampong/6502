cmake_minimum_required(VERSION 3.27)
project(6502)

set(CMAKE_CXX_STANDARD 17)

# Add the main executable
add_executable(6502 6502/main.cpp)

# Include FetchContent module
include(FetchContent)

# Declare Google Test
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Make Google Test available
FetchContent_MakeAvailable(googletest)

# Add the test executable
add_executable(test_6502
        6502/test_6502.cpp
        6502/cpu.cpp

        # Add other source files as needed
)

target_link_libraries(test_6502
        gtest
        gtest_main
        # Add other libraries as needed
)
# Ensure the include directories are set
target_include_directories(test_6502 PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_BINARY_DIR})

# Enable testing
enable_testing()

# Add a test
add_test(NAME test_6502 COMMAND test_6502)