# libcet
A Simple Implementation of Command-Event-Task structure

## 1. How to build libcet
Use cmake to construct solution-files or make-files.

Windows

- a) Create a new directory named "build" under the libcet root path.    
`mkdir build && cd build`

- b) CMake visual stuido solution. (using visual studio 2017 for example)   
`cmake -G "Visual Studio 15 2017" ../src/`   
Tips: If you want to choose other -G flag params, you can type `cmake --help` and see the supported params.

- c) Buid the libcet via visual studio.