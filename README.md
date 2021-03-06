# C++ GameBoy Emulator

GameBoy emulator written in C++.

![Tetris](https://i.imgur.com/c9wvqNY.png) ![Dr. Mario](https://i.imgur.com/IUtulaC.png)

How to Use:
- Clone the repository.
- Run `make` in the `src` folder. If getting an error, make sure to use [this](https://www.ics.uci.edu/~pattis/common/handouts/mingweclipse/mingw.html) version of `g++`
- Run the `gameboy` executable, specifying the path to the ROM as a command line argument. Note only games that use no memory banking or MBC1 are playable.

Controls:
- Arrow Keys &rarr; Directional Pad
- X &rarr; A
- Z &rarr; B
- Space &rarr; Select
- Enter &rarr; Start

Implemented features:
- CPU
- Memory Bus and MBC1
- GPU/PPU and Rendering
- Timer
- Joypad and Keyboard Input

To Do:
- Serial Link
- Other MBC modes
- Audio
- GameBoy Color support
- Pass all Blargg and Mooneye tests
- Saving game states
- Simple GUI for loading games

Resources Used:
- [A journey into GameBoy emulation](https://robertovaccari.com/blog/2020_09_26_gameboy/) - The main reference used, most of the implementations are based off the commentary and source code here.
- [codeslinger GameBoy blog](http://www.codeslinger.co.uk/pages/projects/gameboy/banking.html) - Additional reference, used for details about graphics processing.
- [GameBoy Emulation in JavaScript](https://imrannazar.com/GameBoy-Emulation-in-JavaScript:-The-CPU) - Also used for video details.
- [PanDocs](https://gbdev.io/pandocs/) - Used for details about specific ambiguous behaviour.
- [gbops](https://izik1.github.io/gbops/index.html) - An opcode table used for CPU emulation.
