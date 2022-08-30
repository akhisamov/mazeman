#version 330 core
in vec2 uv;

out vec4 out_color;

uniform sampler2D image;
uniform vec4 color;

void main()
{
    out_color = texelFetch(image, ivec2(uv), 0) * color;
}
