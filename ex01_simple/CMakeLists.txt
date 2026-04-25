cmake_minimum_required(VERSION 3.10)
project(ex01_simple LANGUAGES C)

# Executavel
include_directories(../packages/irobot/)
add_executable(ex01_simple ./main.c ../packages/irobot/irobot.c)