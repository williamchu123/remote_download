cmake_minimum_required(VERSION 3.16)
project(remote_download C)

set(CMAKE_C_STANDARD 99)




add_executable(remote_download main.c call.c call.h utils.c utils.h conc.c conc.h)