add_executable(installer main.cpp ${UTILS_FILES} resources.qrc)

find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)

pkg_check_modules(DtkWidget REQUIRED dtkwidget)

include_directories(${CMAKE_SOURCE_DIR}/utils/)

target_include_directories(installer PUBLIC
    ${DtkWidget_INCLUDE_DIRS}
    ${Qt5DBus_INCLUDE_DIRS}
)

target_link_libraries(installer
    ${Qt5Widgets_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    ${Qt5DBus_LIBRARIES}
)


install(TARGETS installer DESTINATION lib/deepin-graphics-driver-manager)
install(FILES installer-waraper DESTINATION lib/deepin-graphics-driver-manager PERMISSIONS OWNER_EXECUTE OWNER_READ GROUP_READ WORLD_READ)
install(FILES deepin-gradvrmgr-installer.desktop
        DESTINATION lib/deepin-graphics-driver-manager)
