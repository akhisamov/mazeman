#version 330 core
layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_uv;

out vec2 uv;

uniform mat4 transform;
uniform float radian;
uniform vec2 origin;

vec2 rotate(vec2 v, float a)
{
    float s = sin(a);
    float c = cos(a);
    mat2 m = mat2(c, -s, s, c);
    return m * v;
}

void main()
{
    vec2 position = rotate(in_position, radian);
    position += origin;
    gl_Position = transform * vec4(position, 0, 1);

    uv = in_uv;
}
