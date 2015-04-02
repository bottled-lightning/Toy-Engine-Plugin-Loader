cmake_minimum_required(VERSION 2.8)
project(pluginLoader)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.54 COMPONENTS REQUIRED program_options)

include_directories(${pluginLoader_SOURCE_DIR}/include)

add_library(pluginLoader
  src/glPluginLoader.cpp
  ${backward_ENABLE}
)
