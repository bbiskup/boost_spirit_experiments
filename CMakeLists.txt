project(boost_spirit_experiments)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})


# clang settings
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -O3 -I/usr/include/SDL2")
set(CMAKE_EXE_LINKER_FLAGS "-lSDL2 -lSDL2_image")


find_package (Boost COMPONENTS 
    system
 REQUIRED)

 target_link_libraries(${PROJECT_NAME}
                      ${Boost_LIBRARIES}
                      )
