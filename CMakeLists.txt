cmake_minimum_required(VERSION 3.16)

project(TetrisGame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 【改动1】在 find_package 里加上 Sql 和 Multimedia
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Multimedia)

# 【改动2】把 PROJECT_SOURCES 改成更灵活的方式
# 以后每添加一个 .cpp/.h，只需要在这里加一行就行
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        databasehelper.cpp
        databasehelper.h
        tetromino.cpp
        tetromino.h
        tetrominofactory.h
        gamemodel.cpp
        gamemodel.h
        gamecontroller.cpp
        gamecontroller.h
        gamewidget.cpp
        gamewidget.h
        previewwidget.cpp
        previewwidget.h
        logindialog.cpp
        logindialog.h
        rankdialog.cpp
        rankdialog.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TetrisGame
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        databasehelper.h databasehelper.cpp
        tetromino.h tetromino.cpp
        gamemodel.h gamemodel.cpp
        tetrominofactory.h
        gamecontroller.h gamecontroller.cpp
        gamewidget.h gamewidget.cpp
        previewwidget.h previewwidget.cpp
        logindialog.h logindialog.cpp
        rankdialog.h rankdialog.cpp
    )
else()
    if(ANDROID)
        add_library(TetrisGame SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(TetrisGame
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 【改动3】链接库也要加上 Sql 和 Multimedia
target_link_libraries(TetrisGame PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Multimedia
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TetrisGame)
endif()
set_target_properties(TetrisGame PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TetrisGame
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TetrisGame)
endif()
