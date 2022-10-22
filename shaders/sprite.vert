#version 330 core
layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_uv;

out vec2 uv;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(in_position, 0, 1);

    uv = in_uv;
}
