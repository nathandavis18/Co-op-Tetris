# Co-Op Tetris

![Gameplay Demo Image](Images/Gameplay.png?raw=true "Gameplay")

This is a multiplayer version of Tetris that supports up to 4 players!

Currently, there is no way to change the controls, though that is a feature I am currently working on.

To choose the number of players, just click 1, 2, 3, or 4 on your keyboard. The game starts immediately after as there is currently no Main Menu GUI.

The game board size scales with the number of players. One player has a normal sized Tetris board, and it scales linearly to double the size for 4 players!

## Current Features
    Support for up to 4 players
    Control system read from text file
    Default control system
    Full Tetris Game
        Line clearing
        Hard Drops
        Level system
        Music
        etc.

## Features In Progress
    Main Menu GUI
        This will include the functionality to change controls and how many people are playing from the GUI
    High Score System
    
## Current Controls

#### Player One
    Left Arrow -> Left
    Right Arrow -> Right
    Up Arrow -> Rotate
    Down Arrow -> Soft Drop
    Page Down/Space -> Hard Drop
    Delete -> Hold Piece

#### Player Two
    A -> Left
    D -> Right
    W -> Rotate
    S -> Soft Drop
    R -> Hard Drop
    Q -> Hold Piece

#### Player Three
    J -> Left
    L -> Right
    I -> Rotate
    K -> Soft Drop
    P -> Hard Drop
    U -> Hold Piece

#### Player Four
    Numpad4 -> Left
    Numpad6 -> Right
    Numpad8 -> Rotate
    Numpad5 -> Soft Drop
    Numpad+ -> Hard Drop
    Numpad7 -> Hold Piece

    or alternatively if you have a controller 
    (Only tested with Nintendo Switch Controller since that is all I have)

    DPad Left -> Left
    DPad Right -> Right
    X -> Rotate
    DPad Down -> Soft Drop
    A -> Hard Drop
    Y -> Hold Piece

## Building
CMake is the build system for this project. You will need CMake Version 3.16 or later to build.

To build this project, run

    cmake -B {buildDirectory} -S {pathToSource} -DCMAKE_BUILD_TYPE:STRING="Release"
    cmake --build {buildDirectory}

If you are in the source directory (the folder that contains the CMakeLists.txt file), then you can just run

    cmake -B {buildDirectory} -DCMAKE_BUILD_TYPE:STRING="Release"
    cmake --build {buildDirectory}

An example of building (while in the source directory) would be:

    cmake -B "./out/build" -DCMAKE_BUILD_TYPE:STRING="Release"
    cmake --build "./out/build"

After building the project, you will find the executable in the /bin or /bin/Debug folder of the {buildDirectory} specified above.

This has only been tested on Windows, using both Ninja for Visual Studio and the Visual Studio 17 2022 build generators and the MSVC C++ compiler. Untested on Linux and Mac and other C++ compilers. 
