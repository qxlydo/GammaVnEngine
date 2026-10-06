// самописные функции для удобства
#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>

namespace lydo {
    class GameObject {
    public:
        virtual ~GameObject() = default;
        virtual void update(float deltaTime) = 0;
        virtual void render(SDL_Renderer* renderer) = 0;
    };


    struct Application {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        Uint64 lastTime = 0;
        bool vsyncEnabled = false;

        // Список всех объектов
        std::vector<std::unique_ptr<GameObject>> objects;

        // добавлять объекты
        void addObject(std::unique_ptr<GameObject> obj) {
            objects.push_back(std::move(obj));
        }
    };

	struct Vector2f {
		float x;
		float y;
	};
}
