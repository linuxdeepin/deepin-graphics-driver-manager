cmake_minimum_required(VERSION 3.7)

project(deepin-graphics-driver-manager
        VERSION 1.0.0)

set(BIN_NAME deepin-graphics-driver-manager)
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS -Wall)

if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    set(CMAKE_CXX_FLAGS -O3)

    # generate qm
    execute_process(COMMAND bash "translate_generation.sh"
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
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
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

pkg_check_modules(DtkWidget REQUIRED dtkwidget)
pkg_check_modules(PCI REQUIRED libpci)

# driver-manager
add_executable(${BIN_NAME} ${SRCS} resources.qrc)
target_include_directories(${BIN_NAME} PUBLIC ${DtkWidget_INCLUDE_DIRS} ${PCI_INCLUDE_DIRS})
target_link_libraries(${BIN_NAME}
    ${DtkWidget_LIBRARIES}
    ${PCI_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
)

# gltest
add_executable(gltest gltest/main.cpp)
target_link_libraries(gltest ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${Qt5Widgets_LIBRARIES})

# Test settings
enable_testing()
add_executable(Test tests/test.cpp
                    graphicsdeviceinfo.h graphicsdeviceinfo.cpp
                    resolutions/resolution.h resolutions/resolution.cpp
)
target_link_libraries(Test
    ${Qt5Widgets_LIBRARIES}
    ${PCI_LIBRARIES}
)
add_test(NAME Test COMMAND Test)

# Install settings
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX /usr)
endif ()

# bin
install(TARGETS ${BIN_NAME} DESTINATION bin)
install(TARGETS gltest DESTINATION lib/deepin-graphics-driver-manager)

# resolutions.json
install(FILES resources/config/resolutions/resolutions.json
        DESTINATION lib/deepin-graphics-driver-manager)

# systemd servies
install(FILES resources/service/driver-installer.service
        DESTINATION /lib/systemd/system)

# scripts
file(GLOB SCRIPTS "scripts/*.sh")
install(FILES ${SCRIPTS}
        DESTINATION lib/deepin-graphics-driver-manager
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# qm files
file(GLOB QM_FILES "translations/*.qm")
install(FILES ${QM_FILES}
        DESTINATION share/deepin-graphics-driver-manager/translations)