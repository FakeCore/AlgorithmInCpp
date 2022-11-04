cmake_minimum_required(VERSION 3.16.1)

set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)


set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS "-O0 -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -w -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -ggdb")


add_executable("ringbuf" ringbuf_main.cc ringbuf.h)
add_executable("vector" vector.cc)

