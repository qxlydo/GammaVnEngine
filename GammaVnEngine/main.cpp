#include <SDL3/SDL.h>
#include <iostream>
#include "ScreenSize.hpp"

int main(int argc, char* argv[]) {
    std::cout << getScreenSize().x<< "\n";
    std::cout << getScreenSize().y<< "\n";

    SDL_Quit();
    return 0;
}
