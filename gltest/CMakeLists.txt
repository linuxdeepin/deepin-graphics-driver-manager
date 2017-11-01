add_executable(gltest main.cpp)
target_link_libraries(gltest ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${Qt5Widgets_LIBRARIES})


install(TARGETS gltest DESTINATION lib/deepin-graphics-driver-manager)
