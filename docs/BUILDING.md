# Building

## Requirements

- Windows 10 or 11
- Visual Studio with the Desktop development with C++ workload
- CMake 3.24 or newer
- Git

raylib 5.0 is fetched automatically at the commit pinned in `CMakeLists.txt`.

## Compile

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release --parallel
```

Output:

```text
build/Release/ArinsAdventure.exe
```

## Runtime assets

The executable expects these directories beside it:

```text
audio/
map/
sprite/
```

They are intentionally excluded from the public repository because the original game uses third-party art with separate redistribution terms.

If you have an authorized local copy of the assets, place those three directories at the repository root before building. CMake copies them beside the executable automatically.

Tagged release builds are assembled separately from the public source tree after asset-distribution checks.
