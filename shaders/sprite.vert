#version 330 core
layout (location = 0) in vec2 position;

out vec2 TexCoords;

uniform mat4 MVP;

void main()
{
    TexCoords = position;
    gl_Position = MVP * vec4(position.xy, 0.0, 1.0);
}
