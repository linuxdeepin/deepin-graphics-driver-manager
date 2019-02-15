add_executable(installer main.cpp resources.qrc)

find_package(Qt5DBus REQUIRED)

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
install(FILES deepin-gradvrmgr-installer.desktop
        DESTINATION lib/deepin-graphics-driver-manager)
