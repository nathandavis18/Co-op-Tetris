# Co-Op Tetris

![Gameplay Demo Image](Images/Gameplay.png?raw=true "Gameplay")

This is a multiplayer version of Tetris that supports up to 4 players! Game is playable, though to change the controls and number of players you currently have to change them directly in the code.

The game board size scales with the number of players, so 1 player is normal tetris size, 2 players is slightly bigger, etc.

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
    Page Down -> Hard Drop

    or

    Space -> Hard Drop

#### Player Two
    A -> Left
    D -> Right
    W -> Rotate
    S -> Soft Drop
    R -> Hard Drop

#### Player Three
    J -> Left
    L -> Right
    I -> Rotate
    K -> Soft Drop
    P -> Hard Drop

#### Player Four
    Numpad4 -> Left
    Numpad6 -> Right
    Numpad8 -> Rotate
    Numpad5 -> Soft Drop
    Numpad+ -> Hard Drop

    or alternatively if you have a controller

    DPad Left -> Left
    DPad Right -> Right
    X -> Rotate
    DPad Down -> Soft Drop
    A -> Hard Drop



CMake is the build system for this project, for your convenience. You will need vcpkg and cmake Version 3.16 or later to build.
