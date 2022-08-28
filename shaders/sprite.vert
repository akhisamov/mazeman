#version 330 core
layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec4 in_color;

out vec2 uv;
out vec4 color;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(in_position, 0, 1);

    uv = in_uv;
    color = in_color;
}
