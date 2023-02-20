#pragma once

#include <glm/ext/vector_int2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <string_view>

#include "Inari/IService.h"

namespace inari {
    class GameTime;

    class Window final : public IService {
        friend class BaseGame;
        friend class Renderer;

    public:
        void display();

        void setWindowSize(const glm::ivec2& size);
        glm::ivec2 getWindowSize() const;

        void setTitle(const std::string_view& title);

        void setFrameLimit(int screenFps);

        [[nodiscard]] const float& getFrameRate() const { return m_frameRate; }

    protected:
        static std::shared_ptr<Window> create(const std::string_view& title, int width, int height);

        void begin();
        void end(const GameTime& gameTime);

        void* getWindow() const;

    public:
        explicit Window(std::unique_ptr<struct WindowData>&& data);
        ~Window() override;

    private:
        std::unique_ptr<struct WindowData> m_data;

        std::unique_ptr<int> m_frameLimit;

        float m_frameRate { 0.0f };
    };
} // namespace inari
