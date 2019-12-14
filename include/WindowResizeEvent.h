#pragma once

#include <stdint.h>

namespace dss
{

struct WindowResizeEvent {
    uint32_t newWidth;
    uint32_t newHeight;

    WindowResizeEvent(uint32_t width, uint32_t height) :
        newWidth(width),
        newHeight(height) {}
};

}