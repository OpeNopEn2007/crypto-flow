cmake_minimum_required(VERSION 3.20)
project(CryptoFlow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/controlpanel.cpp
    src/crypto/caesar.cpp
    src/crypto/rsa.cpp
    src/scenes/caesarscene.cpp
    src/scenes/rsascene.cpp
)

set(HEADERS
    src/mainwindow.h
    src/controlpanel.h
    src/crypto/caesar.h
    src/crypto/rsa.h
    src/scenes/caesarscene.h
    src/scenes/rsascene.h
)

set(RESOURCES resources/resources.qrc)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
