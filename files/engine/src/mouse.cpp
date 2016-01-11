#include "engine/mouse.hpp"

Mouse::Mouse(){
    lastX = 960;
    lastY = 540;
    hasJustClick = false;
}

void Mouse::printMouse(){
    std::cout << "X : " << lastX << " | Y : " << lastY << std::endl;
}
