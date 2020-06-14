project(HTTP)

cmake_minimum_required(VERSION 3.10)

add_definitions("-g -DDEBUG")

include_directories(src src/protocol src/util)
include_directories(3rdparty/Protocol 3rdparty/LSJson/include 3rdparty/Buffer 3rdparty/Buffer)
include_directories(project/classes/dao project/classes/service project/classes/controller project/classes/servlet project/classes/pojo)

aux_source_directory(project/classes/pojo SRC_POJO)
aux_source_directory(project/classes/service SRC_SERVICE)
aux_source_directory(project/classes/controller SRC_CONTROLLER)
aux_source_directory(project/classes/dao SRC_DAO)
aux_source_directory(project/classes/servlet SRC_SERVLET)
aux_source_directory(src SRC)
aux_source_directory(src/protocol SRC_PROTOCOL)
aux_source_directory(src/util SRC_UTIL)
aux_source_directory(3rdparty/Buffer SRC_BUFFER)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/LSJson/lib)

add_executable(main ${SRC} ${SRC_SERVLET} ${SRC_PROTOCOL} ${SRC_POJO} ${SRC_SERVICE} ${SRC_DAO} ${SRC_CONTROLLER} ${SRC_UTIL} ${SRC_BUFFER})

target_link_libraries(main pthread lsjson)
