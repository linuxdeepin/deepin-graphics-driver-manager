add_executable(notify main.cpp)
target_include_directories(notify PUBLIC ${DtkWidget_INCLUDE_DIRS})
target_link_libraries(notify ${Qt5Widgets_LIBRARIES} ${DtkWidget_LIBRARIES})

install(TARGETS notify DESTINATION lib/deepin-graphics-driver-manager)
install(FILES deepin-gradvrmgr-notify.desktop
        DESTINATION /etc/xdg/autostart)
