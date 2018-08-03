cmake_minimum_required (VERSION 2.8)
project(libcet)
add_definitions(-std=c++11)
aux_source_directory(impl DIR_SRCS)

add_library(cet SHARED ${DIR_SRCS})

install(TARGETS cet DESTINATION bin)
install(DIRECTORY inc DESTINATION "include/libcet")