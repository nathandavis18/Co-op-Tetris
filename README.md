# Co-Op Tetris

![Gameplay Demo Image](Images/Gameplay.png?raw=true "Gameplay")

This is a multiplayer version of Tetris that supports up to 4 players! Game is playable, though to change the controls and number of players you currently have to change them directly in the code.

The game board size scales with the number of players, so 1 player is normal tetris size, 2 players is slightly bigger, etc.

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

While this control system needs to be improved, it is hard to get four hands on the keyboard. Adjustments will be made in the future to allow users to change their controls without diving into the code.

CMake is the build system for this project, for your convenience. You will need vcpkg and cmake Version 3.16 or later to build.
