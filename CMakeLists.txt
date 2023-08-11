cmake_minimum_required(VERSION 3.16)

project(hutsulshchyna-android VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Core Gui Qml SerialPort QuickControls2)
#find_package(REQUIRED COMPONENTS CorePrivate)

qt_standard_project_setup()

if(CMAKE_SYSTEM_NAME MATCHES "Android")
    set(SOURCE_FILES "serialport-handler-android.cpp" "serialport-handler-android.h")
    add_definitions(-DPLATFORM_ANDROID)
else()
    set(SOURCE_FILES "serialport-handler.cpp" "serialport-handler.h")
endif()

qt_add_executable(apphutsulshchyna-android
    main.cpp
    coord-handler.cpp coord-handler.h
    file-handler.h file-handler.cpp
    ${SOURCE_FILES}
    MANUAL_FINALIZATION
)

qt_add_qml_module(apphutsulshchyna-android
    URI hutsulshchyna-android
    VERSION 1.0
    QML_FILES
        "qml/Main.qml"
        "qml/InitialItem.qml"
        "qml/serialport-settings.qml"
        "qml/+android/serialport-settings.qml"
        "qml/terminal.qml"
        "qml/Toast.qml"
        "qml/ToastManager.qml"
    RESOURCES
        "icons/disconnected.svg"
        "icons/connected.svg"
        "icons/coords.png"
        "icons/header.jpg"
        "icons/menu-vertical.svg"
        "icons/menu-burger.svg"
        "icons/arrow-back.svg"
        "icons/settings.svg"
)

set_target_properties(apphutsulshchyna-android PROPERTIES
    QT_ANDROID_MANIFEST "${CMAKE_CURRENT_SOURCE_DIR}/android/AndroidManifest.xml"
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set_property(TARGET apphutsulshchyna-android APPEND PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

target_link_libraries(apphutsulshchyna-android
    PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::SerialPort Qt6::QuickControls2 Qt::CorePrivate
)

install(TARGETS apphutsulshchyna-android
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_finalize_executable(apphutsulshchyna-android)
