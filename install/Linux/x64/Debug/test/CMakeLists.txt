cmake_minimum_required(VERSION 3.14)
project(openvx_test)
#helloworld
LINK_LIBRARIES(${CMAKE_CURRENT_SOURCE_DIR}/../bin/libopenvx.so)
LINK_LIBRARIES(${CMAKE_CURRENT_SOURCE_DIR}/../bin/libvxu.so)
add_executable(helloworld helloworld.cpp)
target_include_directories(helloworld PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
# LINK_LIBRARIES(helloworld PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../bin/libopenvx.so)

#test_second
add_executable(test_second test_second.c)
target_include_directories(test_second PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#test_thrid
add_executable(test_thrid test_thrid.cpp)
target_include_directories(test_thrid PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#test_forth
add_executable(test_forth test_forth.cpp)
target_include_directories(test_forth PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#test
add_executable(test test.cpp)
target_include_directories(test PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
# LINK_LIBRARIES(helloworld PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../bin/libopenvx.so)

#zhihu_test
add_executable(zhihu_test zhihu_test.cpp)
target_include_directories(zhihu_test PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
# LINK_LIBRARIES(helloworld PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../bin/libopenvx.so)