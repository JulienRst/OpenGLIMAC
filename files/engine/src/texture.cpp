#include "engine/texture.hpp"

using namespace glimac;
using namespace std;

HTexture::HTexture(FilePath path){
    std::unique_ptr<Image> img = loadImage(path);
    if(img == NULL)
        std::cerr << "Echec du chargement de la Texture" << std::endl;

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

GLuint textureToDisplay(FilePath page,std::map<std::string,std::unique_ptr<HTexture>> const &m,Mouse &mouse){
    if(page == "home")
        return textureOfHome(m,mouse);
    if(page == "sound")
        return textureOfSound(m,mouse);
    if(page == "credit")
        return textureOfCredit(m,mouse);
    if(page == "play")
        return textureOfPlay(m,mouse);
    if(page == "help")
        return textureOfHelp(m,mouse);
    return -1;
}

GLuint textureOfHelp(std::map<std::string,unique_ptr<HTexture>> const &m,Mouse &mouse){
    if(mouse.lastX > 435 && mouse.lastX < 745 && mouse.lastY > 320 && mouse.lastY < 450)
        return m.at("back_help")->getTextureIndice();

    return m.at("main_help")->getTextureIndice();
}

GLuint textureOfPlay(std::map<std::string,unique_ptr<HTexture>> const &m,Mouse &mouse){
    if(mouse.lastX > 435 && mouse.lastX < 745 && mouse.lastY > 320 && mouse.lastY < 450)
        return m.at("play_menu_back")->getTextureIndice();
    if(mouse.lastX > 800 && mouse.lastX < 1110 && mouse.lastY > 320 && mouse.lastY < 450)
        return m.at("play_menu_help")->getTextureIndice();
    if(mouse.lastX > 800 && mouse.lastX < 1110 && mouse.lastY > 475 && mouse.lastY < 610)
        return m.at("play_menu_go")->getTextureIndice();

    return m.at("main_menu_play")->getTextureIndice();
}

GLuint textureOfCredit(std::map<std::string,unique_ptr<HTexture>> const &m,Mouse &mouse){
    if(mouse.lastX > 435 && mouse.lastX < 745 && mouse.lastY > 320 && mouse.lastY < 450)
        return m.at("back_credit")->getTextureIndice();

    return m.at("main_credit")->getTextureIndice();
}

GLuint textureOfSound(std::map<std::string,unique_ptr<HTexture>> const &m,Mouse &mouse){
    if(mouse.lastX > 435 && mouse.lastX < 745 && mouse.lastY > 320 && mouse.lastY < 450)
        return m.at("back_sound_menu")->getTextureIndice();
    if(mouse.lastX > 800 && mouse.lastX < 1110 && mouse.lastY > 320 && mouse.lastY < 450)
        return m.at("on_sound_menu")->getTextureIndice();
    if(mouse.lastX > 800 && mouse.lastX < 1110 && mouse.lastY > 475 && mouse.lastY < 610)
        return m.at("off_sound_menu")->getTextureIndice();

    return m.at("main_sound_menu")->getTextureIndice();
}

GLuint textureOfHome(std::map<std::string,unique_ptr<HTexture>> const &m,Mouse &mouse){
    if(mouse.lastX > 805 && mouse.lastX < 1115 && mouse.lastY > 320 && mouse.lastY < 450)
        return m.at("play_hover")->getTextureIndice();
    if(mouse.lastX > 805 && mouse.lastX < 1115 && mouse.lastY > 475 && mouse.lastY < 605)
        return m.at("sound_hover")->getTextureIndice();
    if(mouse.lastX > 805 && mouse.lastX < 1115 && mouse.lastY > 635 && mouse.lastY < 765)
        return m.at("credit_hover")->getTextureIndice();
    if(mouse.lastX > 805 && mouse.lastX < 1115 && mouse.lastY > 795 && mouse.lastY < 925)
        return m.at("exit_hover")->getTextureIndice();

    return m.at("main_menu")->getTextureIndice();
}
