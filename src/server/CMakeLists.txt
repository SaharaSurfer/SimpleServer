cmake_minimum_required(VERSION 3.10)
project(SimpleServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

set(SOURCES
  main.cc
  bookstore_database.cc
  server.cc
  session.cc
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/header)

find_package(Boost 1.74 REQUIRED COMPONENTS system thread)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PUBLIC _GLIBCXX_USE_CXX11_ABI=1)

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  pqxx
  pq
)