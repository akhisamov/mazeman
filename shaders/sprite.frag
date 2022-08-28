#version 330 core
in vec2 uv;
in vec4 color;

out vec4 out_color;

uniform sampler2D image;

void main()
{
    out_color = texelFetch(image, ivec2(uv), 0) * color;
}
