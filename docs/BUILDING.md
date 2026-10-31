# Building Arin's Adventure

## Requirements

- Windows 10 or 11
- Visual Studio 2022 with the Desktop development with C++ workload
- CMake 3.24 or newer
- Git

raylib 5.0 is fetched automatically by CMake at the pinned commit used by this repository.

## Build

From the repository root:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release --parallel
```

The executable is written to:

```text
build/Release/ArinsAdventure.exe
```

The game also needs the `audio`, `map`, and `sprite` directories beside the executable. The CMake build copies those directories automatically when they are present in the repository.

## Clean rebuild

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build -A x64
cmake --build build --config Release --parallel
```

## Notes

The clean repository intentionally does not use the obsolete Box2D and fmt NuGet dependencies from the original coursework project because the game source does not use them.
