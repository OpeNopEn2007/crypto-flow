cmake_minimum_required(VERSION 3.20)
project(CryptoFlow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/controlpanel.cpp
    src/crypto/caesar.cpp
    src/crypto/rsa.cpp
    src/crypto/vigenere.cpp
    src/scenes/caesarscene.cpp
    src/scenes/rsascene.cpp
    src/scenes/vigenerecene.cpp
    src/scenes/base64scene.cpp
    src/scenes/xorscene.cpp
)

set(HEADERS
    src/mainwindow.h
    src/controlpanel.h
    src/crypto/caesar.h
    src/crypto/rsa.h
    src/crypto/vigenere.h
    src/scenes/caesarscene.h
    src/scenes/rsascene.h
    src/scenes/vigenerecene.h
    src/scenes/base64scene.h
    src/scenes/xorscene.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
