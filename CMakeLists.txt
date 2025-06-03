# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(videowidget LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(INSTALL_DIR "E:/Workspaces/QTProjects/videowidget")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Multimedia MultimediaWidgets Widgets)

qt_add_executable(videowidget
    main.cpp
    videoplayer.cpp videoplayer.h
)

set_target_properties(videowidget PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

qt_add_ios_ffmpeg_libraries(videowidget)

target_link_libraries(videowidget PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Multimedia
    Qt::MultimediaWidgets
    Qt::Widgets
)

install(TARGETS videowidget
    RUNTIME DESTINATION "${INSTALL_DIR}"
    BUNDLE DESTINATION "${INSTALL_DIR}"
    LIBRARY DESTINATION "${INSTALL_DIR}"
)
