#define SDL_MAIN_USE_CALLBACKS 1 // Включаем режим колбэков SDL3
#include <SDL3/SDL_main.h>       // Обязательный заголовок для этого режима
#include <SDL3/SDL.h>
#include <iostream>
#include "core.hpp"
#include "ScreenSize.hpp"

// 1. Инициализация (вызывается ОДИН раз при старте)
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Создаем наше приложение в динамической памяти
    auto app = std::make_unique<lydo::Application>();

    if (!SDL_CreateWindowAndRenderer("SDL3 Clean Architecture", getScreenSize().x, getScreenSize().y, SDL_WINDOW_RESIZABLE, &app->window, &app->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Ограничиваем FPS через VSync
    if (SDL_SetRenderVSync(app->renderer, 1)) {
        app->vsyncEnabled = true;
    }

    app->lastTime = SDL_GetTicksNS();

    // ==========================================
    // МЕСТО ДЛЯ ДОБАВЛЕНИЯ ВАШИХ ОБЪЕКТОВ
    // app->addObject(std::make_unique<MySuperObject>());
    // ==========================================

    // Передаем указатель на наше приложение в систему SDL
    *appstate = app.release();
    return SDL_APP_CONTINUE;
}

// 2. Обработка событий (вызывается каждый раз, когда что-то происходит)
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    auto* app = static_cast<lydo::Application*>(appstate);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; // Завершает приложение штатно
    }

    // Здесь можно передавать события конкретным объектам, если нужно
    return SDL_APP_CONTINUE;
}

// 3. Обновление и Отрисовка (вызывается циклически каждый кадр)
SDL_AppResult SDL_AppIterate(void* appstate) {
    auto* app = static_cast<lydo::Application*>(appstate);

    // Считаем Delta Time (время между кадрами в секундах) для плавной работы
    Uint64 currentTime = SDL_GetTicksNS();
    float deltaTime = static_cast<float>(currentTime - app->lastTime) / 1'000'000'000.0f;
    app->lastTime = currentTime;

    // --- Ограничение FPS (если аппаратный VSync по какой-то причине не сработал) ---
    if (!app->vsyncEnabled) {
        const float targetFPS = 60.0f;
        const float targetNS = 1.0f / targetFPS;
        if (deltaTime < targetNS) {
            SDL_DelayNS(static_cast<Uint64>((targetNS - deltaTime) * 1'000'000'000.0f));
            return SDL_APP_CONTINUE; // Пропускаем итерацию, ждем тайминга
        }
    }

    // --- Логика (Update) ---
    for (auto& obj : app->objects) {
        obj->update(deltaTime);
    }

    // --- Отрисовка (Render) ---
    SDL_SetRenderDrawColor(app->renderer, 20, 20, 30, 255); // Красивый темно-синий фон
    SDL_RenderClear(app->renderer);

    // Отрисовываем все добавленные объекты автоматически
    for (auto& obj : app->objects) {
        obj->render(app->renderer);
    }

    SDL_RenderPresent(app->renderer);

    return SDL_APP_CONTINUE;
}

// 4. Очистка памяти (вызывается ОДИН раз при закрытии)
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    if (appstate) {
        auto* app = static_cast<lydo::Application*>(appstate);
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        delete app; // Удаляем структуру приложения и все объекты внутри неё
    }
    SDL_Quit();
}
