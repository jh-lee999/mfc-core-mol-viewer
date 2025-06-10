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

struct ColorRGB {
    float r, g, b;
};

ColorRGB GetColor(ColorName name) {
    switch (name) {
    case ColorName::Red: return { 1.0f, 0.0f, 0.0f };
    case ColorName::Green: return { 0.0f, 1.0f, 0.0f };
    case ColorName::Blue: return { 0.0f, 0.0f, 1.0f };
    case ColorName::Yellow: return { 1.0f, 1.0f, 0.0f };
    case ColorName::Cyan: return { 0.0f, 1.0f, 1.0f };
    case ColorName::Magenta: return { 1.0f, 0.0f, 1.0f };
    case ColorName::White: return { 1.0f, 1.0f, 1.0f };
    case ColorName::Black: return { 0.0f, 0.0f, 0.0f };
    case ColorName::Gray: return { 0.5f, 0.5f, 0.5f };
    default: return { 1.0f, 1.0f, 1.0f };
    }
}
