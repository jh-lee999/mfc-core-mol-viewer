#pragma once
#include "pch.h"
enum class ColorName {
    Red,
    Green,
    Blue,
    Yellow,
    Cyan,
    Magenta,
    White,
    Black,
    Gray
};

struct ColorRGB { float r, g, b; };

ColorRGB GetColor(ColorName name);

