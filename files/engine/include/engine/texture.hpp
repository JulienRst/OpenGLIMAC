#include <cstdlib>
#include <iostream>
#include <glimac/Image.hpp>
#include <GL/glew.h>
#include <memory>
#include <map>

#include "engine/mouse.hpp"


class HTexture {
    private:
        GLuint texture;
    public:
        HTexture(glimac::FilePath path);
        ~HTexture();
        GLuint getTextureIndice();
};

GLuint textureToDisplay(std::map<std::string,unique_ptr<HTexture>> const &m,Mouse &mouse);
