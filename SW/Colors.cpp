#pragma once
#include "pch.h"
#include "Colors.h"


ColorRGB GetColor(ColorName name)
{
    switch (name) {
    case ColorName::Red:     return { 1.0f, 0.0f, 0.0f };
    case ColorName::Green:   return { 0.0f, 1.0f, 0.0f };
    case ColorName::Blue:    return { 0.0f, 0.0f, 1.0f };
    case ColorName::Yellow:  return { 1.0f, 1.0f, 0.0f };
    case ColorName::Cyan:    return { 0.0f, 1.0f, 1.0f };
    case ColorName::Magenta: return { 1.0f, 0.0f, 1.0f };
    case ColorName::White:   return { 1.0f, 1.0f, 1.0f };
    case ColorName::Black:   return { 0.0f, 0.0f, 0.0f };
    case ColorName::Gray:    return { 0.5f, 0.5f, 0.5f };
    case ColorName::Orange:  return { 1.0f, 0.5f, 0.0f };
    case ColorName::Pink:    return { 1.0f, 0.75f, 0.8f };
    case ColorName::Brown:   return { 0.6f, 0.3f, 0.0f };
    case ColorName::Purple:  return { 0.5f, 0.0f, 0.5f };
    case ColorName::Teal:    return { 0.0f, 0.5f, 0.5f };
    case ColorName::Olive:   return { 0.5f, 0.5f, 0.0f };
    case ColorName::Navy:    return { 0.0f, 0.0f, 0.5f };
    case ColorName::Silver:  return { 0.75f, 0.75f, 0.75f };
    case ColorName::Gold:    return { 1.0f, 0.84f, 0.0f };
    default:                 return { 1.0f, 1.0f, 1.0f }; // fallback
    }
}

ColorName StringToColorName(const std::string& name) {
    if (name == "Red")     return ColorName::Red;
    if (name == "Green")   return ColorName::Green;
    if (name == "Blue")    return ColorName::Blue;
    if (name == "Yellow")  return ColorName::Yellow;
    if (name == "Cyan")    return ColorName::Cyan;
    if (name == "Magenta") return ColorName::Magenta;
    if (name == "White")   return ColorName::White;
    if (name == "Black")   return ColorName::Black;
    if (name == "Gray")    return ColorName::Gray;
    if (name == "Orange")  return ColorName::Orange;
    if (name == "Pink")    return ColorName::Pink;
    if (name == "Brown")   return ColorName::Brown;
    if (name == "Purple")  return ColorName::Purple;
    if (name == "Teal")    return ColorName::Teal;
    if (name == "Olive")   return ColorName::Olive;
    if (name == "Navy")    return ColorName::Navy;
    if (name == "Silver")  return ColorName::Silver;
    if (name == "Gold")    return ColorName::Gold;
    return ColorName::White; // fallback
}
