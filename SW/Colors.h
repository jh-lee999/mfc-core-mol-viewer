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
    Gray,
    Orange,
    Pink,
    Brown,
    Purple,
    Teal,
    Olive,
    Navy,
    Silver,
    Gold
};

struct ColorRGB { float r, g, b; };

ColorRGB GetColor(ColorName name);
ColorName StringToColorName(const std::string& name);
