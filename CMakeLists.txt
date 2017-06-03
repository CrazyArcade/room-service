cmake_minimum_required(VERSION 3.2)
project(server)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(Threads)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB MAP_FILES ${PROJECT_SOURCE_DIR}/src/maps/*.json)
file(COPY ${MAP_FILES} DESTINATION maps)

set(SOURCE_FILES src/main.cpp src/model/entity.cpp src/model/entity.h src/model/player.cpp src/model/player.h src/utils/utils.h src/app.cpp src/app.h src/controller/roomController.cpp src/controller/roomController.h src/utils/log.h src/settings.h src/utils/objectid.h src/app.h src/model/bubble.cpp src/model/bubble.h src/model/map.cpp src/model/map.h src/room.h src/room.cpp)
add_executable(server ${SOURCE_FILES})

target_link_libraries(server pthread)
target_link_libraries(server uuid)
target_link_libraries(server uWS)
target_link_libraries(server ssl)
target_link_libraries(server crypto)
target_link_libraries(server z)