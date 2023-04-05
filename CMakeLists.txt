

cmake_minimum_required (VERSION 3.11)
project (ISM330DHCX_CXX VERSION 1.0.0 DESCRIPTION "A C++ library for ism330dhcx imu sensor--Odroid, RaspberryPi..")


set (CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


INCLUDE_DIRECTORIES(./include)
INCLUDE_DIRECTORIES(/usr/include/wiringpi2)

set (LIBSOURCES
	./src/ism330dhcx_reg.c 
	./src/ism330dhcx.cpp
	./src/c_wire_.cpp
	./src/wire_.cpp
)


add_library(${PROJECT_NAME} ${LIBSOURCES})

add_executable (simple_example ${CMAKE_SOURCE_DIR}/examples/simple_example/simple_example.cpp)
LINK_DIRECTORIES($(CMAKE_LIBRARY_OUTPUT_DIRECTORY))
TARGET_LINK_LIBRARIES(simple_example -lwiringPiDev -lwiringPi ${PROJECT_NAME})