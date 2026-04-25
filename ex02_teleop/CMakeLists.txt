cmake_minimum_required(VERSION 3.10)
project(ex02_teleop LANGUAGES C)

# Executavel
include_directories(../packages/irobot/)
add_executable(ex02_teleop ./main.c ../packages/irobot/irobot.c)