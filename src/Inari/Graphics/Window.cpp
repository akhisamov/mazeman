#include "Window.h"

#include <SDL.h>

#include <stdexcept>

#include "Inari/Utils/GameTime.h"
#include "Inari/Utils/Strings.h"

namespace inari {
    struct WindowData {
        SDL_Window* window = nullptr;

        uint32_t totalFrames = 0;
        uint64_t startCounter = 0;
    };

    std::shared_ptr<Window> Window::create(const std::string_view& title, int width, int height)
    {
        auto data = std::make_unique<WindowData>();
        data->window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                                        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
        if (data->window == nullptr) {
            return nullptr;
        }

        return std::make_shared<Window>(std::move(data));
    }

    Window::Window(std::unique_ptr<struct WindowData>&& data)
        : m_data(std::move(data))
        , m_frameLimit(nullptr)
    {
    }

    Window::~Window() { SDL_DestroyWindow(m_data->window); }

    void Window::display() { SDL_GL_SwapWindow(m_data->window); }

    void Window::setWindowSize(const glm::ivec2& size)
    {
        if (m_data->window) {
            SDL_SetWindowSize(m_data->window, size.x, size.y);
        }
    }

    glm::ivec2 Window::getWindowSize() const
    {
        glm::ivec2 size {};
        if (m_data->window) {
            SDL_GetWindowSize(m_data->window, &size.x, &size.y);
        }
        return size;
    }

    void Window::setTitle(const std::string_view& title)
    {
        if (m_data->window) {
            SDL_SetWindowTitle(m_data->window, title.data());
        }
    }

    void Window::setFrameLimit(int screenFps) { m_frameLimit = std::make_unique<int>(screenFps); }

    void Window::begin()
    {
        m_data->totalFrames++;
        m_data->startCounter = SDL_GetPerformanceCounter();
    }

    void Window::end(const GameTime& gameTime)
    {
        m_frameRate = static_cast<float>(m_data->totalFrames) / gameTime.getTotalMs();
        if (m_data->totalFrames > 20000) {
            m_data->totalFrames = 0;
        }
        const uint64_t endCounter = SDL_GetPerformanceCounter();
        if (m_frameLimit) {
            const float screenTicksPerFrame = 1000.0f / static_cast<float>(*m_frameLimit);
            const float elapsedMs = static_cast<float>(endCounter - m_data->startCounter)
                / static_cast<float>(SDL_GetPerformanceFrequency()) * 1000.0f;
            if (screenTicksPerFrame > elapsedMs) {
                SDL_Delay(static_cast<uint32_t>(screenTicksPerFrame - elapsedMs));
            }
        }
    }

    void* Window::getWindow() const
    {
        if (m_data) {
            return m_data->window;
        }
        return nullptr;
    }
} // namespace inari
