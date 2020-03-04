#  Assignment 4: Automated Testing

## Build and Run Tests

**CMake is required for build! Follow instructions below to install**

Getting the source files:

`git clone https://github.com/Andrey-Korn/GameOfLife-Curses.git`

Go to the directory:

`cd GameOfLife-Curses`

Building cmake:

`mkdir cmake-build && cd cmake-build && cmake ..`

Building game executable:

`make`

Running tests:

`./Assignment4Test`

Launch the Game!

`./GameOfLife`

## Install libncurses

### Linux

**In most distrs, the library will already be preinstalled.**

Use the package manager of your distr. Example for Debian:

`sudo apt-get update`

`sudo apt-get install libncurses5-dev`

### Mac OS X

Use the [Homebrew](https://brew.sh) package manager:

`brew install ncurses`

### Windows

**Without mouse support.**

- [PDCurses](https://pdcurses.sourceforge.io)

- ~~Install to Windows 10 tools from Ubuntu?~~


