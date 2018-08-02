cmake_minimum_required (VERSION 2.8)
project(libcet)

aux_source_directory(./impl DIR_SRCS)
add_subdirectory(./impl/ext-networks)

add_library(libcet SHARED ${DIR_SRCS})

target_link_libraries(libcet libcet_ext_networks)