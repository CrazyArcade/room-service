cmake_minimum_required(VERSION 3.7)
project(server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS program_options system)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES src/main.cpp src/model/entity.cpp src/model/entity.h src/model/player.cpp src/model/player.h src/utils.h src/utils.cpp src/api.cpp src/api.h)
add_executable(server ${SOURCE_FILES})

target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(server pthread)
target_link_libraries(server uuid)