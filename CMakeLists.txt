# Minimum version of CMake
cmake_minimum_required(VERSION 3.28.3)

# Project name
project(BancaApp)

# Compiler settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
add_executable(BancaApp main.cpp ContBancar.cpp)

# Display message during compilation
message(STATUS "Compiling project BancaApp...")

# Ensure `make clean` removes the executable
set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "BancaApp")
