cmake_minimum_required(VERSION 3.2)
project(server)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(Threads)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES src/main.cpp src/model/entity.cpp src/model/entity.h src/model/player.cpp src/model/player.h src/utils.h src/utils.cpp src/socket_bind.cpp src/socket_bind.h src/controller/game.cpp src/controller/game.h src/controller/room.cpp src/controller/room.h src/utils/log.h src/settings.h)
add_executable(server ${SOURCE_FILES})

target_link_libraries(server pthread)
target_link_libraries(server uuid)
target_link_libraries(server uWS)
target_link_libraries(server ssl)
target_link_libraries(server crypto)
target_link_libraries(server z)