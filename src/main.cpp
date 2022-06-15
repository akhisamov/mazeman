#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "Texture2D.hpp"
#include "SpriteRenderer.hpp"
#include "ResourceManager.hpp"

void logError()
{
    std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        logError();
        return -1;
    }

    {
        const int flags = IMG_INIT_JPG | IMG_INIT_PNG;
        const int initted = IMG_Init(flags);
        if ((initted & flags) != flags)
        {
            std::cerr << "IMG_Error: " << IMG_GetError() << std::endl;
            return -1;
        }
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow("Pac Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        logError();
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr)
    {
        logError();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize context" << std::endl;
        return -1;
    }

    auto resources = std::make_shared<ResourceManager>("resources.db");
    auto texture = resources->load<Texture2D>("wall");
    if (texture == nullptr)
    {
        return -1;
    }

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

        renderer->draw(texture);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
