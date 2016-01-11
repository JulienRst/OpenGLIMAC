#pragma once

#include <cstdlib>
#include <iostream>
#include "engine/music.hpp"
#include <string>

using namespace std;

class Mouse {
public:
    float lastX;
    float lastY;
    bool hasJustClick;

    Mouse();
    void printMouse();
};
