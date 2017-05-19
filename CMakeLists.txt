cmake_minimum_required(VERSION 3.2)
project(server)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(Threads)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES src/main.cpp src/model/entity.cpp src/model/entity.h src/model/player.cpp src/model/player.h src/utils/utils.h src/socket_bind.cpp src/socket_bind.h src/controller/roomController.cpp src/controller/roomController.h src/utils/log.h src/settings.h src/utils/objectid.h src/model/map/base.h src/app.h src/model/map/town_10.h src/controller/mapController.cpp src/controller/mapController.h)
add_executable(server ${SOURCE_FILES})

target_link_libraries(server pthread)
target_link_libraries(server uuid)
target_link_libraries(server uWS)
target_link_libraries(server ssl)
target_link_libraries(server crypto)
target_link_libraries(server z)