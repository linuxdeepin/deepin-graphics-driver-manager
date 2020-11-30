
set(BIN_NAME deepin-graphics-driver-manager-server)


# Defines path to resource directories
if (CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions("-DSCRIPTS_DIR=\"${CMAKE_SOURCE_DIR}/scripts/\"")
  add_definitions("-DRESOLUTIONS_DIR=\"${CMAKE_SOURCE_DIR}/server/resources/config/resolutions/\"")
else()
  add_definitions("-DSCRIPTS_DIR=\"${CMAKE_INSTALL_PREFIX}/lib/deepin-graphics-driver-manager/\"")
  add_definitions("-DRESOLUTIONS_DIR=\"${CMAKE_INSTALL_PREFIX}/lib/deepin-graphics-driver-manager/\"")
endif()


find_package(PkgConfig REQUIRED)
find_package(Qt5DBus REQUIRED)

pkg_check_modules(PCI REQUIRED libpci)
pkg_check_modules(DtkWidget REQUIRED dtkwidget)

file(GLOB SRCS "*.h" "*.cpp")
add_executable(${BIN_NAME} ${SRCS})

target_include_directories(${BIN_NAME} PUBLIC
    ${DtkWidget_INCLUDE_DIRS}
    ${PCI_INCLUDE_DIRS}
    ${Qt5DBus_INCLUDE_DIRS}
)

target_link_libraries(${BIN_NAME}
    ${PCI_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    ${Qt5DBus_LIBRARIES}
)
