project(boost_spirit_experiments)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(num_list num_list.cpp)
add_executable(actions actions.cpp)
add_executable(complex_number complex_number.cpp)
add_executable(num_list2 num_list2.cpp)
add_executable(roman roman.cpp)
add_executable(boost_book boost_book.cpp)
add_executable(c64assembler c64assembler.cpp)


# clang settings
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -O3 -I/usr/include/SDL2")
set(CMAKE_EXE_LINKER_FLAGS "-lSDL2 -lSDL2_image")


find_package (Boost COMPONENTS 
    system
 REQUIRED)

 target_link_libraries(num_list ${Boost_LIBRARIES})
 target_link_libraries(actions ${Boost_LIBRARIES})
 target_link_libraries(complex_number ${Boost_LIBRARIES})
 target_link_libraries(num_list2 ${Boost_LIBRARIES})
 target_link_libraries(roman ${Boost_LIBRARIES})
 target_link_libraries(boost_book ${Boost_LIBRARIES})
 target_link_libraries(c64assembler ${Boost_LIBRARIES})