cmake_minimum_required(VERSION 3.7)

if (NOT DEFINED VERSION)
    set(VERSION 1.0.0)
endif()

project(deepin-graphics-driver-manager)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fpie -pie")

# Install settings

set(CMAKE_INSTALL_PREFIX /usr)


execute_process(COMMAND bash "-c" "qdbuscpp2xml -A server/graphicsdriverinterface.h -o com.deepin.daemon.GraphicsDriver.xml"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

execute_process(COMMAND bash "-c" "qdbusxml2cpp ../com.deepin.daemon.GraphicsDriver.xml -i graphicsdriverinterface.h -a graphicsdriveradaptor"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/server)

execute_process(COMMAND bash "-c" "qdbusxml2cpp ../com.deepin.daemon.GraphicsDriver.xml -p graphicsdriverproxy"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/utils)


if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
endif ()

message("translate_generation.sh")
# generate qm
execute_process(COMMAND bash "translate_generation.sh"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
execute_process(COMMAND bash "translate_ts2desktop.sh"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})



file(GLOB UTILS_FILES "${CMAKE_SOURCE_DIR}/utils/*.h" "${CMAKE_SOURCE_DIR}/utils/*.cpp")

add_subdirectory(server)
add_subdirectory(prepare)
add_subdirectory(installer)
add_subdirectory(gltest)

# Install help document
#install(DIRECTORY assets/deepin-graphics-driver-manager
#    DESTINATION /usr/share/deepin-manual/manual-assets/application/)

