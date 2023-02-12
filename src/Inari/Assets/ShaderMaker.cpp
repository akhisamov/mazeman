#include "ShaderMaker.h"

#include <glad/glad.h>

#include "Inari/Assets/Shader.h"
#include "Inari/Utils/Strings.h"

#include <iostream>

namespace {
    bool checkShaderStatus(uint32_t id, GLenum type)
    {
        int success = 0;
        if (type == GL_COMPILE_STATUS) {
            glGetShaderiv(id, type, &success);
        } else {
            glGetProgramiv(id, type, &success);
        }
        if (!success) {
            char infoLog[512];
            if (type == GL_COMPILE_STATUS) {
                glGetShaderInfoLog(id, 512, nullptr, infoLog);
            } else {
                glGetProgramInfoLog(id, 512, nullptr, infoLog);
            }
            std::cerr << "Shader error: " << infoLog << std::endl;
            return false;
        }
        return true;
    }

    constexpr std::string_view shaderFormat = "#version 330 core\n%s\n%s";
}

namespace inari {
    std::shared_ptr<IAsset> ShaderMaker::createAsset(const std::string_view& data)
    {
        const std::string vertexShader = strings::format(shaderFormat, "#define VERTEX_SHADER", data.data());
        const std::string fragmentShader = strings::format(shaderFormat, "#define FRAGMENT_SHADER", data.data());

        return createAsset(vertexShader, fragmentShader);
    }

    std::shared_ptr<IAsset> ShaderMaker::createAsset(const std::string_view& vertexCode,
                                                     const std::string_view& fragmentCode)
    {
        Shader::Data data;
        uint32_t vertexShader = 0;
        uint32_t fragmentShader = 0;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexCodeStr = vertexCode.data();
        glShaderSource(vertexShader, 1, &vertexCodeStr, nullptr);
        glCompileShader(vertexShader);
        if (checkShaderStatus(vertexShader, GL_COMPILE_STATUS)) {
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            const char* fragmentCodeStr = fragmentCode.data();
            glShaderSource(fragmentShader, 1, &fragmentCodeStr, nullptr);
            glCompileShader(fragmentShader);

            if (checkShaderStatus(fragmentShader, GL_COMPILE_STATUS)) {
                data.id = glCreateProgram();
                glAttachShader(data.id, vertexShader);
                glAttachShader(data.id, fragmentShader);
                glLinkProgram(data.id);

                if (!checkShaderStatus(data.id, GL_LINK_STATUS)) {
                    data.id = 0;
                }
            }
        }

        if (vertexShader != 0) {
            glDeleteShader(vertexShader);
        }
        if (fragmentShader != 0) {
            glDeleteShader(fragmentShader);
        }
        if (data.id != 0) {
            return std::make_shared<Shader>(data);
        }

        return nullptr;
    }
}