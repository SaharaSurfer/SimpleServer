cmake_minimum_required(VERSION 3.10)
project(SimpleServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(SRC_LIST main.cc bookstore_database.cc)
include_directories("header/")

find_package(Boost 1.74 REQUIRED COMPONENTS system thread)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} -lpqxx -lpq)
