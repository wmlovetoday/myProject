# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.5)

# 项目信息
project (${PRO_NAME})

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++14 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

set(CMAKE_VERBOSE_MAKEFILE true)

if(CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "=============== build release ==============")
else()
  set(CMAKE_BUILD_TYPE Debug)
  message(STATUS "=============== build debug ==============")
endif()

if(NOT ARM)
  set(THIRD third)
  message(STATUS "=============== build x64 ==============")
else()
  set(THIRD third_arm)
  message(STATUS "=============== build arm ==============")
endif()

message(STATUS "PROJECT_SOURCE_DIR=" ${PROJECT_SOURCE_DIR}) # CMAKE.TXT DIR

set(fmt_header_dir ${PROJECT_SOURCE_DIR}/../${THIRD}/fmt)
set(fmt_lib_dir    ${PROJECT_SOURCE_DIR}/../${THIRD}/fmt)

set(zlog_header_dir ${PROJECT_SOURCE_DIR}/../${THIRD}/zlog)
set(zlog_lib_dir    ${PROJECT_SOURCE_DIR}/../${THIRD}/zlog)

set(zmq_header_dir ${PROJECT_SOURCE_DIR}/../${THIRD}/zmq/include)
set(zmq_lib_dir    ${PROJECT_SOURCE_DIR}/../${THIRD}/zmq/libzmq/lib)

set(opencv_header_dir /home/code/installation_package/ubuntu/install_opencv3/opencv-3.3.1/opencv_3/include)
set(opencv_lib_dir    /home/code/installation_package/ubuntu/install_opencv3/opencv-3.3.1/opencv_3/lib)

list(APPEND lib_files "")
list(APPEND header_files ${PROJECT_SOURCE_DIR}/../include)

if(ENABLE_ZLOG)
  include_directories(${PROJECT_SOURCE_DIR}/../src/log)
  include_directories(${zlog_header_dir} ${fmt_header_dir})
  link_directories(${fmt_lib_dir} ${zlog_lib_dir})
  list(APPEND lib_files "pt_zlog " "fmt ")
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/log SourceList)
  message(STATUS "=============== build zlog ==============")
endif()

if(ENABLE_ZMQ_DDS)
  include_directories(${PROJECT_SOURCE_DIR}/../src/zmq)
  include_directories(${PROJECT_SOURCE_DIR}/../src/dds)
  include_directories(${PROJECT_SOURCE_DIR}/../src/dds/dds_zmq)
  include_directories(${zmq_header_dir})
  link_directories(${zmq_lib_dir})
  list(APPEND lib_files "zmq " )
  add_definitions(-DZMQ_DDS)
  message(STATUS "=============== build zmq ==============")
endif()

if(ENABLE_OPENCV)
include_directories(${opencv_header_dir})
link_directories(${opencv_lib_dir})
list(APPEND lib_files opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_videoio opencv_photo)
message(STATUS "=============== build opencv ==============")
endif()

message(STATUS "***************** lib_files=" ${lib_files})
message(STATUS "***************** header_files=" ${header_files})

add_definitions(-DPRT_TEST)
include_directories(${PRO_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../include)

# link_directories(${fmt_lib_dir} ${zlog_lib_dir})

aux_source_directory( ${PRO_DIR} SourceList)
if(ENABLE_ZMQ_DDS)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/zmq SourceList)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/dds SourceList)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/dds/dds_zmq SourceList)
endif()  
add_executable(${PROJECT_NAME} ${SourceList})

target_link_libraries(${PROJECT_NAME} ${lib_files} pthread)

install(PROGRAMS ${PRO_DIR}/build/${PROJECT_NAME} DESTINATION ${PRO_DIR} )
