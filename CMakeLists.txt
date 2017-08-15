cmake_minimum_required(VERSION 3.7)

project(deepin-graphics-driver-manager
        VERSION 1.0.0)

set(BIN_NAME deepin-graphics-driver-manager)
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS -Wall)

if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    set(CMAKE_CXX_FLAGS -O3)
else ()
    set(CMAKE_CXX_COMPILER "clang++")
endif ()

# Sources files
file(GLOB SRCS "*.h" "*.cpp"
               "widgets/*.h" "widgets/*.cpp"
               "resolutions/*.h" "resolutions/*.cpp")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)

pkg_check_modules(DtkWidget REQUIRED dtkwidget)

# Tell CMake to create the executable
add_executable(${BIN_NAME} ${SRCS})
target_include_directories(${BIN_NAME} PUBLIC ${DtkWidget_INCLUDE_DIRS})
target_link_libraries(${BIN_NAME}
    ${DtkWidget_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
)

# Install settings
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

install(TARGETS ${BIN_NAME} DESTINATION bin)

# Test settings
enable_testing()
add_executable(Test tests/test.cpp
                    graphicsdeviceinfo.h graphicsdeviceinfo.cpp
                    resolutions/resolution.h resolutions/resolution.cpp
)
target_link_libraries(Test
${Qt5Widgets_LIBRARIES}
)
add_test(NAME Test COMMAND Test)
