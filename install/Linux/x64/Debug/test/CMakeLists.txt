cmake_minimum_required(VERSION 3.14)
project(openvx_test)
#helloworld
LINK_LIBRARIES(${CMAKE_CURRENT_SOURCE_DIR}/../bin/libopenvx.so)
LINK_LIBRARIES(${CMAKE_CURRENT_SOURCE_DIR}/../bin/libvxu.so)
add_executable(helloworld helloworld.cpp)
target_include_directories(helloworld PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)

#test_second
add_executable(test_second test_second.c)
target_include_directories(test_second PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#test_thrid
add_executable(test_thrid test_thrid.cpp)
target_include_directories(test_thrid PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#test_forth
# add_executable(test_forth test_forth.cpp)
# target_include_directories(test_forth PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#test
add_executable(test test.cpp)
target_include_directories(test PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)


#zhihu_test
add_executable(zhihu_test zhihu_test.cpp)
target_include_directories(zhihu_test PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#vx_pyramid
add_executable(vx_pyramid vx_pyramid.cpp)
target_include_directories(vx_pyramid PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)

#vx_node
add_executable(vx_node vx_node.cpp)
target_include_directories(vx_node PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#vx_matrix
add_executable(vx_matrix vx_matrix.cpp)
target_include_directories(vx_matrix PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#vx_lut
add_executable(vx_lut vx_lut.cpp)
target_include_directories(vx_lut PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)

#vx_image
add_executable(vx_image vx_image.cpp)
target_include_directories(vx_image PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#vx_graph
add_executable(vx_graph vx_graph.cpp)
target_include_directories(vx_graph PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#vx_context
add_executable(vx_context vx_context.cpp)
target_include_directories(vx_context PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#vx_graph
add_executable(vx_arrary vx_arrary.cpp)
target_include_directories(vx_arrary PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)