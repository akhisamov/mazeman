#include "Window.hpp"

#include <glad/glad.h>

#include <SDL.h>

#include <stdexcept>

#include "Utils/Strings.hpp"

struct WindowData
{
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;

    void create(const std::string_view& title, int width, int height)
    {
        window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        if (window == nullptr)
        {
            throw std::runtime_error(strings::format("Window Creation Error: %s", SDL_GetError()));
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        glContext = SDL_GL_CreateContext(window);
        if (glContext == nullptr)
        {
            throw std::runtime_error(strings::format("GL Context Creation Error: %s", SDL_GetError()));
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            throw std::runtime_error("Failed to initialize glad context");
        }
    }

    void destroy() const
    {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
    }
};

Window::Window(const std::string_view& title, int width, int height)
    : m_data(std::make_unique<WindowData>())
{
    m_data->create(title, width, height);
}

Window::~Window()
{
    m_data->destroy();
    SDL_Quit();
}

void Window::display() { SDL_GL_SwapWindow(m_data->window); }

Window::Size Window::getWindowSize() const
{
    Size size;
    if (m_data->window)
    {
        SDL_GetWindowSize(m_data->window, &size.width, &size.height);
    }
    return size;
}

void Window::setTitle(const std::string_view& title)
{
    if (m_data->window)
    {
        SDL_SetWindowTitle(m_data->window, title.data());
    }
}
