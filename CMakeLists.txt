cmake_minimum_required(VERSION 3.16)

project(hutsulshchyna-android VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Core Gui Qml SerialPort QuickControls2)

qt_standard_project_setup()

qt_add_executable(apphutsulshchyna-android
    main.cpp
    serialport-handler.cpp serialport-handler.h
)

qt_add_qml_module(apphutsulshchyna-android
    URI hutsulshchyna-android
    VERSION 1.0
    QML_FILES
        "qml/Main.qml"
        "qml/serialport-settings.qml"
        "qml/serialport-settings-new.qml"
        "qml/serialport-settings-old.qml"
        "qml/terminal.qml"
    RESOURCES
        "icons/back.png"
        "icons/drawer.png"
        "icons/menu.png"
        "icons/disconnect.png"
        "icons/connect.png"
        "icons/settings.png"
)

set_target_properties(apphutsulshchyna-android PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apphutsulshchyna-android
    PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::SerialPort Qt6::QuickControls2
)

install(TARGETS apphutsulshchyna-android
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
