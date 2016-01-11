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

GLuint textureToDisplay(glimac::FilePath page,std::map<std::string,std::unique_ptr<HTexture>> const &m,Mouse &mouse);

GLuint textureOfHelp(std::map<std::string,std::unique_ptr<HTexture>> const &m,Mouse &mouse);
GLuint textureOfCredit(std::map<std::string,std::unique_ptr<HTexture>> const &m,Mouse &mouse);
GLuint textureOfHome(std::map<std::string,std::unique_ptr<HTexture>> const &m,Mouse &mouse);
GLuint textureOfPlay(std::map<std::string,std::unique_ptr<HTexture>> const &m,Mouse &mouse);
GLuint textureOfSound(std::map<std::string,std::unique_ptr<HTexture>> const &m,Mouse &mouse);
