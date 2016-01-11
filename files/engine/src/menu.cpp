#include "engine/menu.hpp"

using namespace std;

string processMenuAction(string &page,Mouse &mouse){
    if(page == "home"){
        if(mouse.lastX > 805 && mouse.lastX < 1115 && mouse.lastY > 320 && mouse.lastY < 450){
            mouse.hasJustClick = true;
            page = "play";
        }
        if(mouse.lastX > 805 && mouse.lastX < 1115 && mouse.lastY > 475 && mouse.lastY < 605){
            mouse.hasJustClick = true;
            page = "sound";
        }
        if(mouse.lastX > 805 && mouse.lastX < 1115 && mouse.lastY > 635 && mouse.lastY < 765)
            page = "credit";
        if(mouse.lastX > 805 && mouse.lastX < 1115 && mouse.lastY > 795 && mouse.lastY < 925)
            return "quit";
    }

    if(page == "sound"){
        if(mouse.lastX > 435 && mouse.lastX < 745 && mouse.lastY > 320 && mouse.lastY < 450)
            page = "home";
        if(mouse.lastX > 800 && mouse.lastX < 1110 && mouse.lastY > 320 && mouse.lastY < 450)
            return "playSound";
        if(mouse.lastX > 800 && mouse.lastX < 1110 && mouse.lastY > 475 && mouse.lastY < 610){
            if(!mouse.hasJustClick){
                return "pauseSound";
            }
        }
    }

    if(page == "play"){
        if(mouse.lastX > 435 && mouse.lastX < 745 && mouse.lastY > 320 && mouse.lastY < 450)
            if(!mouse.hasJustClick){
                page = "home";
            }
        if(mouse.lastX > 800 && mouse.lastX < 1110 && mouse.lastY > 320 && mouse.lastY < 450){
            if(!mouse.hasJustClick){
                page = "help";
            }
        }
        if(mouse.lastX > 800 && mouse.lastX < 1110 && mouse.lastY > 475 && mouse.lastY < 610)
            return "play";
    }

    if(page == "credit"){
        if(mouse.lastX > 435 && mouse.lastX < 745 && mouse.lastY > 320 && mouse.lastY < 450)
            page = "home";
    }

    if(page == "help"){
        if(mouse.lastX > 435 && mouse.lastX < 745 && mouse.lastY > 320 && mouse.lastY < 450){
            mouse.hasJustClick = true;
            page = "play";
        }
    }

    return "";
}
