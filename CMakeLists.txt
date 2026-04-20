cmake_minimum_required(VERSION 3.10)

# The name of your project
project(ClankerShooter)

# Use C++17 (standard for modern projects)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# List ALL your .cpp files here so the CI knows what to compile
# Example: add_executable(ClankerShooter main.cpp OtherFile.cpp)
add_executable(ClankerShooter main.cpp)