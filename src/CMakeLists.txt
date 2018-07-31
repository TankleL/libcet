cmake_minimum_required (VERSION 2.8)
project(libcet)
aux_source_directory(./impl DIR_SRCS)
add_library(libcet SHARED ${DIR_SRCS})