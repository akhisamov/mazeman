#version 330 core
layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec4 in_color;

uniform mat4 transform;

out vec2 out_uv;
out vec4 out_color;

void main()
{
    out_uv = in_uv;
    out_color = in_color;

    gl_Position = transform * vec4(in_position, 0, 1);
}
