#include "engine/texture.hpp"

using namespace glimac;

HTexture::HTexture(FilePath path){
    std::unique_ptr<Image> img = loadImage(path);
    if(img != NULL){
        std::cerr << "Chargement de la main_menu OK" << std::endl;
    } else {
        std::cerr << "Echec du chargement de la main_menu" << std::endl;
    }

    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->getWidth(),img->getHeight(),0,GL_RGBA,GL_FLOAT,img->getPixels());
        // APPLYING FILTER
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);
}

HTexture::~HTexture(){
    glDeleteTextures(1,&texture);
}

GLuint HTexture::getTextureIndice(){
    return texture;
}

GLuint textureToDisplay(std::map<std::string,unique_ptr<HTexture>> const &m,Mouse &mouse){
    if(mouse.lastX > 805 && mouse.lastX < 1115 && mouse.lastY > 320 && mouse.lastY < 450)
        return m.at("play_hover")->getTextureIndice();

    return m.at("main_menu")->getTextureIndice();
}
