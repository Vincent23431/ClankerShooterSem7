cmake_minimum_required(VERSION 3.10)
project(ClankerShooter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Since this file will now be inside "Sem7HiTech_ClankerShooter", 
# the path to your code is just "src/main.cpp"
add_executable(ClankerShooter src/main.cpp)