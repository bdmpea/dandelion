cmake_minimum_required(VERSION 3.5)

project(First_Project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(PostgreSQL REQUIRED)
find_package(libpqxx REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt5 REQUIRED COMPONENTS Network)

set(PROJECT_SOURCES
        cllient/include/client.h
        cllient/include/main_menu.h
        cllient/include/main_window.h
        cllient/include/balda.h
        cllient/include/info_window.h
        cllient/include/vocabulary_list.h
        cllient/include/letter.h
        cllient/src/info_window.cpp
        cllient/src/info_window.ui
        cllient/src/balda.cpp
        cllient/src/vocabulary_list.cpp
        cllient/src/letter.cpp
        cllient/src/balda.ui
        cllient/src/vocabulary_list.ui
        cllient/include/button.h
        cllient/include/cards_for_training.h
        cllient/include/flashcard.h
        cllient/include/flashcards_desk.h
        cllient/include/personal_account.h
        cllient/include/registration.h
        cllient/include/training.h
        cllient/include/welcome.h
        cllient/src/welcome.cpp
        cllient/src/training.cpp
        cllient/src/registration.cpp
        cllient/src/personal_account.cpp
        cllient/src/main_window.cpp
        cllient/src/main_menu.cpp
        cllient/src/main.cpp
        cllient/src/flashcards_desk.cpp
        cllient/src/flashcard.cpp
        cllient/src/cards_for_training.cpp
        cllient/src/button.ui
        cllient/src/flashcard.ui
        cllient/src/flashcards_desk.ui
        cllient/src/main_menu.ui
        cllient/src/main_window.ui
        cllient/src/personal_account.ui
        cllient/src/registration.ui
        cllient/src/training.ui
        cllient/src/welcome.ui
        cllient/src/button.cpp
        cllient/include/client.h
        cllient/src/client.cpp
        server/include/database.hpp
        server/include/server.hpp
        server/src/server.cpp
        server/src/database.cpp
        cllient/resources/imgs.qrc
        )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(First_Project
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET First_Project APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(First_Project SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(First_Project
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(First_Project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(First_Project PRIVATE pqxx)

target_link_libraries(First_Project
        PRIVATE libpqxx::pqxx
        PRIVATE PostgreSQL::PostgreSQL)

target_link_libraries(First_Project PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(First_Project PRIVATE Qt5::Network)

set_target_properties(First_Project PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS First_Project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(First_Project)
endif()
