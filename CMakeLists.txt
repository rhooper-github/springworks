cmake_minimum_required (VERSION 2.6)
project (springworks)

include_directories(rapidjson/include)

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.59.0 COMPONENTS date_time)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_library(springworks springworks.h springworks.cpp)
    target_link_libraries(springworks ${Boost_LIBRARIES})
    add_executable(springworks-cli springworks-cli.cpp)
    target_link_libraries(springworks-cli springworks)
endif()

