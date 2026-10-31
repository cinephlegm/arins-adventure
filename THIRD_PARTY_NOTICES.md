# Third-party notices

## Software

### raylib 5.0

- https://github.com/raysan5/raylib
- pinned commit: `ae50bfa2cc569c0f8d5bc4315d39db64005b1b08`
- zlib/libpng license

raylib is fetched by CMake and is not vendored in this repository.

### raygui 4.1-dev

- https://github.com/raysan5/raygui
- zlib/libpng license
- vendored as `third_party/raygui.h`

### raymath 1.5

- https://github.com/raysan5/raylib
- zlib/libpng license
- vendored as `third_party/raymath.h`

### Tileson 1.4.0

- https://github.com/SSBMTonberry/tileson
- BSD 2-Clause license
- vendored as `third_party/tileson.hpp`

Tileson includes json11 under its own permissive license notice. Original notices remain in the vendored header.

## Game assets

Runtime art, maps and audio are tracked separately from the public source tree. See [`ASSETS.md`](ASSETS.md).
