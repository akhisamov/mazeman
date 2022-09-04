#include "Window.hpp"

#include <glad/glad.h>

#include <SDL.h>

#include <stdexcept>

#include "Inari/Utils/Strings.hpp"

namespace inari
{
    struct WindowData
    {
        SDL_Window* window = nullptr;
        SDL_GLContext glContext = nullptr;

        uint64_t startCounter = 0;

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
        , m_frameLimit(nullptr)
    {
        m_data->create(title, width, height);
    }

    Window::~Window()
    {
        m_data->destroy();
        SDL_Quit();
    }

    void Window::clear(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::display() { SDL_GL_SwapWindow(m_data->window); }

    void Window::setWindowSize(const glm::ivec2& size)
    {
        if (m_data->window)
        {
            SDL_SetWindowSize(m_data->window, size.x, size.y);
        }
    }

    glm::ivec2 Window::getWindowSize() const
    {
        glm::ivec2 size {};
        if (m_data->window)
        {
            SDL_GetWindowSize(m_data->window, &size.x, &size.y);
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

    void Window::setFrameLimit(int screenFps) { m_frameLimit = std::make_unique<int>(screenFps); }

    void Window::begin() { m_data->startCounter = SDL_GetPerformanceCounter(); }
    void Window::end()
    {
        const uint64_t endCounter = SDL_GetPerformanceCounter();
        if (m_frameLimit)
        {
            const float screenTicksPerFrame = 1000.0f / (*m_frameLimit);
            const float elapsedMs
                = (endCounter - m_data->startCounter) / static_cast<float>(SDL_GetPerformanceFrequency()) * 1000.0f;
            if (screenTicksPerFrame > elapsedMs)
            {
                SDL_Delay(static_cast<uint32_t>(screenTicksPerFrame - elapsedMs));
            }
        }
    }
}