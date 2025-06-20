cmake_minimum_required(VERSION 3.10)
project(MiniGit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenSSL REQUIRED)

# Add executable
add_executable(minigit 
    src/main.cpp
    src/minigit.cpp
    src/commit.cpp
    src/blob.cpp
    src/branch.cpp
    src/utils.cpp
)

# Include directories
target_include_directories(minigit PRIVATE include)

# Link libraries
target_link_libraries(minigit OpenSSL::SSL OpenSSL::Crypto)

# Set compiler flags
if(MSVC)
    target_compile_options(minigit PRIVATE /W4)
else()
    target_compile_options(minigit PRIVATE -Wall -Wextra -Wpedantic)
endif() 