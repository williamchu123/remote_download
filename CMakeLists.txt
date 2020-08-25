cmake_minimum_required(VERSION 3.16)
project(remote_download C)

set(CMAKE_C_STANDARD 99)

find_package (Threads)

add_executable(remote_download main.c call.c call.h utils.c utils.h conc.c conc.h)

target_link_libraries (remote_download ${CMAKE_THREAD_LIBS_INIT})
