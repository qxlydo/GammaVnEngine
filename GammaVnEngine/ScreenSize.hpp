#pragma once

#include <SDL3/SDL.h>
#include "core.hpp"

lydo::Vector2f getScreenSize() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Ошибка инициализации SDL: " << SDL_GetError() << std::endl;
    }
    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();

    const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displayID);
    if (mode) {
        return { static_cast<float>(mode->w), static_cast<float>(mode->h) };
    }
    else {
        std::cerr << "Не удалось получить размер экрана: " << SDL_GetError() << std::endl;
    }
}