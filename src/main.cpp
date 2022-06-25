#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "ResourceManager/ResourceManager.hpp"

#include "Sprite.hpp"
#include "SpriteRenderer.hpp"
#include "StringUtils.hpp"

int main(int argc, char* argv[])
{
    try
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            throw std::runtime_error(StringUtils::format("SDL Init Error: %s", SDL_GetError()));
        }

        constexpr int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
        if ((IMG_Init(imgFlags) & imgFlags) != imgFlags)
        {
            throw std::runtime_error(StringUtils::format("IMG Init Error: %s", IMG_GetError()));
        }

        SDL_Window* window = SDL_CreateWindow("Pac Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        if (window == nullptr)
        {
            throw std::runtime_error(StringUtils::format("Window Creation Error: %s", SDL_GetError()));
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GLContext glContext = SDL_GL_CreateContext(window);
        if (glContext == nullptr)
        {
            throw std::runtime_error(StringUtils::format("GL Context Creation Error: %s", SDL_GetError()));
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            throw std::runtime_error("Failed to initialize glad context");
        }

        auto resources = std::make_unique<ResourceManager>("res.bundle");

        const auto& sprite = Sprite::create(resources->load<Texture2D>("wall"));
        sprite->setOrigin(0.0f, 0.0f);

        auto renderer = std::make_shared<SpriteRenderer>(resources->load<Shader>("sprite"));

        bool isRunning = true;
        while (isRunning)
        {
            SDL_Event e;
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    isRunning = false;
                }
            }

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            renderer->draw(sprite);

            SDL_GL_SwapWindow(window);
        }

        resources->unload(sprite->getTexture());

        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
