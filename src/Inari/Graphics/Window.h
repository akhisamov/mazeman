#pragma once

#include <glm/ext/vector_int2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <string_view>

#include "Inari/IService.h"

namespace inari {
    class Window final : public IService {
        friend class BaseGame;
        friend class Renderer;

    public:
        void display();

        void setWindowSize(const glm::ivec2& size);
        glm::ivec2 getWindowSize() const;

        void setTitle(const std::string_view& title);

        void setFrameLimit(int screenFps);

    protected:
        static std::shared_ptr<Window> create(const std::string_view& title, int width, int height);

        void begin();
        void end();

        void* getWindow() const;

    public:
        explicit Window(std::unique_ptr<struct WindowData>&& data);
        ~Window() override;

    private:
        std::unique_ptr<struct WindowData> m_data;

        std::unique_ptr<int> m_frameLimit;
    };
} // namespace inari
