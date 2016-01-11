#include "engine/mouse.hpp"

Mouse::Mouse(){
    lastX = 960;
    lastY = 540;
}

void Mouse::printMouse(){
    std::cout << "X : " << lastX << " | Y : " << lastY << std::endl;
}
