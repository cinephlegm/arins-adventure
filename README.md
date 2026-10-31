# Arin's Adventure

A small 2D adventure game written in C++17 with [raylib](https://www.raylib.com/) and maps authored in Tiled.

Originally developed as a university Game Mechanics project, this repository is the clean standalone version of Task 2, separated from the original coursework repository and prepared as a playable portfolio release.

## Features

- Multi-area world with village, cave and dungeon maps
- Objective-driven quest progression
- NPCs and event-driven cutscenes
- Collectibles, keys and temporary power-ups
- Turrets and projectile hazards
- Collision and trigger handling
- Teleportation and screen-shake events
- Music, sound effects and in-game UI
- Tiled map loading through Tileson

## Technology

- C++17
- raylib 5.0
- raygui
- raymath
- Tiled
- Tileson
- Box2D 2.3
- fmt 7
- Visual Studio 2022 in the original project

## Repository status

The original project was stored inside a larger coursework repository alongside an unrelated Task 1. This repository is being rebuilt from that source with a clean history before public release.

The migration will:

1. preserve only Arin's Adventure;
2. remove coursework-specific naming and unrelated files;
3. document third-party asset attribution;
4. replace machine-specific build assumptions with a reproducible build;
5. add automated Windows builds and downloadable GitHub Releases.

## Building

The original build used Visual Studio 2022 and NuGet packages for raylib, Box2D and fmt. A reproducible CMake-based build will replace that setup during the cleanup.

## Release

Windows builds will be published under GitHub Releases once the clean build pipeline is verified.

## License

Licensing is intentionally not declared yet. Source code and third-party game assets will be licensed and attributed separately before this repository becomes public.
