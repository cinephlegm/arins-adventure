#pragma once

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

#include <iostream>
#include "tileson.hpp"

#if defined(_WIN32)
#define NOGDI
#define NOUSER
#endif
