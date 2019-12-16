#pragma once

#include <memory>
#include "Window.h"

namespace dss
{

/**
 *  The coordinate converter will let us encapsulate the functionality to
 *  convert coordinates from "unit" space ([0.0, 1.0], [0.0, 1.0]) to screen space
 *  ([0, window.Width], [0, window.Height]) 
 * 
 *  The class keeps a pointer to the Window and uses its dimensions in it's calculations
 */
class CoordinateConverter {
private:
    std::shared_ptr<Window> _window;

public:
    CoordinateConverter() = delete;
    CoordinateConverter(std::shared_ptr<Window> window) : _window(window) {}

    inline float UnitToScreenSpaceWidth(float unitAmount) const {
        return static_cast<float>(_window->Width()) * unitAmount;
    }

    inline float UnitToScreenSpaceHeight(float unitAmount) const {
        return static_cast<float>(_window->Height()) * unitAmount;
    }

    inline float ScreenSpaceToUnitWidth(float screenAmount) const {
        return screenAmount / static_cast<float>(_window->Width());
    }

    inline float ScreenSpaceToUnitHeight(float screenAmount) const {
        return screenAmount / static_cast<float>(_window->Height());
    }

    inline float ScreenWidth() const {
        return static_cast<float>(_window->Width());
    }

    inline float ScreenHeight() const {
        return static_cast<float>(_window->Height());
    }
};

}