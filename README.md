# raster

Meshes in your terminal.
This is still a work-in-progress, but my final goal is to be able to load OBJ mesh files.

## Installation

You need [ncurses](https://invisible-island.net/ncurses/) in order to build.
It usually comes by default on Linux and macOS, but you can check if you have it by running `tic -V` in your terminal.

Download the header-only dependency [Eigen](https://eigen.tuxfamily.org/dox/) by running,

```
git submodule init
git submodule update
```

To build, run

```
make all
```

## Run

After building, run the app,

```
./bin/main
```

- The mesh uses `wasd` or arrow keys to rotate.
- The `q` key will quit the app.
- The `r` key will refresh the display, e.g. if something caused the game to render incorrectly.
