cmake_minimum_required(VERSION 3.1.0)

project(test-qtwebkit)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 5.4.0 REQUIRED COMPONENTS Core Gui Network WebKitWidgets Widgets)

set(testqtwebkit_src
	main.cpp
	MainWindow.cpp
)

qt5_wrap_ui(testqtwebkit_ui
	MainWindow.ui
)

add_executable(test-qtwebkit
	${testqtwebkit_src}
	${testqtwebkit_ui}
)

target_link_libraries(test-qtwebkit Qt5::Core Qt5::Gui Qt5::Network Qt5::WebKit Qt5::WebKitWidgets Qt5::Widgets)

install(TARGETS test-qtwebkit DESTINATION bin/)
