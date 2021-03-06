# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.5)

# 项目信息
project (${PRO_NAME})

# rov close : -fno-elide-constructors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O2")

set(CMAKE_VERBOSE_MAKEFILE true) # display build details

if(ENABLE_RELEASE)
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "=============== release ${PLATFORM} ${VERSION} ==============")
else()
  message(STATUS "=============== debug ${PLATFORM} ${VERSION} ==============")
  set(CMAKE_BUILD_TYPE Debug)
  add_compile_options(-g -ggdb -fstack-protector-all -march=native -fnon-call-exceptions)
endif()

message(STATUS "PROJECT_SOURCE_DIR=" ${PROJECT_SOURCE_DIR}) # CMAKE.TXT DIR

list(APPEND lib_files "")
list(APPEND header_files ${PROJECT_SOURCE_DIR}/../include)

if(ENABLE_ZLOG)
  set(fmt_header_dir ${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/fmt)
  set(fmt_lib_dir    ${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/fmt)
  
  set(zlog_header_dir ${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/zlog)
  set(zlog_lib_dir    ${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/zlog)

  include_directories(${PROJECT_SOURCE_DIR}/../src/log)
  include_directories(${zlog_header_dir} ${fmt_header_dir})
  link_directories(${fmt_lib_dir} ${zlog_lib_dir})
  list(APPEND lib_files "pt_zlog " "fmt ")
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/log SourceList)
  message(STATUS "=============== build zlog ==============")
endif()

if(ENABLE_ZMQ_DDS)
  set(zmq_header_dir ${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/zmq/include)
  set(zmq_lib_dir    ${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/zmq/libzmq/lib)
  
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
  set(opencv_header_dir ${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/opencv_ubuntu${VERSION}/include)
  set(opencv_lib_dir    ${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/opencv_ubuntu${VERSION}/lib)

  include_directories(${opencv_header_dir})
  link_directories(${opencv_lib_dir})
  list(APPEND lib_files opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_videoio opencv_photo)
  message(STATUS "=============== build opencv ==============")
endif()

message(STATUS "***************** lib_files=" ${lib_files})
message(STATUS "***************** header_files=" ${header_files})

add_definitions(-DPRT_TEST)
include_directories(${PRO_DIR})
include_directories(${PRO_DIR}/../src)
include_directories(${PROJECT_SOURCE_DIR}/../include)
include_directories(${PROJECT_SOURCE_DIR}/../src/common)

if(ENABLE_SYS==ON)
  include_directories(${PROJECT_SOURCE_DIR}/../src/sys)
  include_directories(${PROJECT_SOURCE_DIR}/../src/sys/terminal)
  include_directories(${PROJECT_SOURCE_DIR}/../src/sys/socket)
  include_directories(${PROJECT_SOURCE_DIR}/../src/sys/th)
endif()

if(ENABLE_XML)
  include_directories(${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/pugixml)
endif()

# link_directories(${fmt_lib_dir} ${zlog_lib_dir})

aux_source_directory(${PRO_DIR} SourceList)
aux_source_directory(${PRO_DIR}/../src SourceList)
aux_source_directory(${PROJECT_SOURCE_DIR}/../src/common SourceList)

if(ENABLE_SYS)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/sys SourceList)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/sys/terminal SourceList)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/sys/socket SourceList)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/sys/th SourceList)
endif()

if(ENABLE_XML)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../third/${PLATFORM}/pugixml SourceList)
endif()

if(ENABLE_ZMQ_DDS)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/zmq SourceList)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/dds SourceList)
  aux_source_directory(${PROJECT_SOURCE_DIR}/../src/dds/dds_zmq SourceList)
endif()  

add_executable(${PROJECT_NAME} ${SourceList})

target_link_libraries(${PROJECT_NAME} ${lib_files} pthread)

install(PROGRAMS ${PRO_DIR}/build/${PROJECT_NAME} DESTINATION ${PRO_DIR} )
