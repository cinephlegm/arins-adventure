# Repository instructions

This repository is the clean standalone source for Arin's Adventure.

## Rules

- Preserve game behaviour while modernising the build and project structure.
- Do not reintroduce files from GAME_MECH_TASK1.
- Treat maps, sprites, audio and other game assets as externally referenced resources unless their ownership and redistribution rights are documented.
- Do not rename map, sprite or audio paths without updating every runtime reference.
- Prefer C++17-compatible code unless the build standard is deliberately changed.
- Keep third-party libraries out of the repository when they can be fetched reproducibly by the build system.
- Never commit generated executables, build directories or package caches.
- Any gameplay refactor must preserve the existing quest sequence, map transitions and asset-relative paths unless the change explicitly targets them.

## Validation

Before merging a change:

1. configure a clean build;
2. compile a Windows x64 Release build;
3. launch the game from the packaged directory;
4. verify asset loading and the main menu;
5. verify at least one map transition and one objective update.
