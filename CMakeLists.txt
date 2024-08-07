﻿cmake_minimum_required(VERSION 3.16)
project(Tetris LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(SOURCES src/main.cpp
			src/Game.cpp
			src/Blocks.cpp
			src/Renderer.cpp
            src/MusicController.cpp 
            src/InputController.cpp
            src/Board.cpp
            src/PieceState.cpp
            MainMenu/src/MainMenu.cpp
            MainMenu/src/MainMenuEventHandler.cpp
)

set(HEADERS Headers/Blocks.hpp
			Headers/PieceState.hpp
			Headers/Game.hpp
			Headers/Globals.hpp
			Headers/Renderer.hpp
            Headers/MusicController.hpp
            Headers/InputController.hpp
            Headers/Board.hpp
            MainMenu/Headers/MainMenu.hpp
            MainMenu/Headers/MainMenuEventHandler.hpp
)

add_executable (Tetris ${SOURCES} ${HEADERS})
target_link_libraries(Tetris PRIVATE sfml-graphics sfml-audio)
target_compile_features(Tetris PRIVATE cxx_std_20)

if(WIN32)
    add_custom_command(
        TARGET Tetris
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Tetris>
        VERBATIM)
endif()

add_custom_command(
    TARGET Tetris
    COMMENT "Copy Required Files"
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Controls/default-controls.txt $<TARGET_FILE_DIR:Tetris>
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Font/tetris-font.ttf $<TARGET_FILE_DIR:Tetris>
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Music/Tetris-Theme.ogg $<TARGET_FILE_DIR:Tetris>
    VERBATIM)