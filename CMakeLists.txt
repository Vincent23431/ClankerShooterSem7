cmake_minimum_required(VERSION 3.10)
project(ClankerShooter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This tells CMake to look inside your src folder for the files
# Replace 'main.cpp' with whatever your actual main file is named
add_executable(ClankerShooter code/Sem7HiTech_ClankerShooter/src/main.cpp)

# If you have multiple files, list them all like this:
# add_executable(ClankerShooter src/main.cpp src/GunLogic.cpp src/Utils.cpp)