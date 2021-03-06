# Copyright: (C) 2015 iCub Facility - Istituto Italiano di Tecnologia
# Author: Alessandro Roncone <alessandro.roncone@iit.it>
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

project(reactController)

find_package(IPOPT REQUIRED)

set(header_files ${CMAKE_CURRENT_SOURCE_DIR}/include/reactIpOpt.h
                 ${CMAKE_CURRENT_SOURCE_DIR}/include/reactCtrlThread.h
                 ${CMAKE_CURRENT_SOURCE_DIR}/include/particleThread.h
                 ${CMAKE_CURRENT_SOURCE_DIR}/include/avoidanceHandler.h)
set(source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/reactIpOpt.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/reactCtrlThread.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/particleThread.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/reactController.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/avoidanceHandler.cpp)
set(idl_files    ${PROJECT_NAME}.thrift)

yarp_add_idl(IDL_GEN_FILES ${PROJECT_NAME}.thrift)

source_group("IDL Files" FILES ${idl_files})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${IPOPT_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${header_files} ${source_files} ${IDL_GEN_FILES} ${idl_files})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${IPOPT_DEFINITIONS} _USE_MATH_DEFINES)
target_link_libraries(${PROJECT_NAME} ${YARP_LIBRARIES} ctrlLib iKin skinDynLib ${IPOPT_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " ${IPOPT_LINK_FLAGS}")
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

