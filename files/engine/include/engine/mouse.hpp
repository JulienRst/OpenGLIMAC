#pragma once

#include <cstdlib>
#include <iostream>
#include "engine/music.hpp"
#include <string>

class Mouse {
public:
    float lastX;
    float lastY;
    bool hasJustClick;

    Mouse();
    void printMouse();
};
