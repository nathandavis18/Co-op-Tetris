# Co-Op Tetris

This is a multiplayer version of Tetris that supports up to 4 players! Game is playable, though to change the controls and number of players you currently have to change them directly in the code.

The game m_board size scales with the number of players, so 1 player is normal tetris size, 2 players is slightly bigger, etc.

## Current Controls

#### Player One
    Left Arrow -> Left
    Right Arrow -> Right
    Up Arrow -> Rotate
    Down Arrow -> Soft Drop
    Space -> Hard Drop

#### Player Two
    A -> Left
    D -> Right
    W -> Rotate
    S -> Soft Drop
    R -> Hard Drop

#### Player Three
    Delete -> Left
    Page Down -> Right
    Home -> Rotate
    End -> Soft Drop
    Page Up -> Hard Drop

#### Player Four
    Numpad4 -> Left
    Numpad6 -> Right
    Numpad8 -> Rotate
    Numpad5 -> Soft Drop
    Numpad0 -> Hard Drop

While this control system needs to be improved, it is hard to get four hands on the keym_board. Adjustments will be made in the future to allow users to change their controls without diving into the code.

CMake is the build system for this project, for your convenience. You will need vcpkg and cmake Version 3.16 or later to build.
