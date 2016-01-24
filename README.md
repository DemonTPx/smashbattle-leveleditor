## Smash Battle level editor

![Screenshot of Smash Battle leveleditor working on linux](/screenshot.gif?raw=true)

This tool can be used to create and modify levels for [Smash Battle](https://github.com/DemonTPx/smashbattle).

**NOTE**: Make sure you create/edit levels in the `Battle/stage` folder in the Smash Battle repository. If you don't, the sprite and music files will not be available. 

## Features

- Create and edit levels
- Character sprites and animation tester

## Prerequisites

### On Ubuntu linux

    apt-get install build-essential cmake libwxbase3.0-0 libwxbase3.0-dev zlib1g-dev

### On Mac OSX (using brew)

    brew install wxmac zlib


## Build instructions

    cmake .
    make

## Run instructions

**NOTE**: Make sure the working directory is `SB_LevelEdit` when running the executable. For instance:

    cd ./SB\_LevelEdit/
    ../leveleditor
