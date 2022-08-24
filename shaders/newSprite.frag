#version 330 core
in vec2 in_uv;
in vec4 in_color;

uniform sampler2D image;

out vec4 out_color;

void main()
{
    out_color = in_color * texture(image, in_uv);
}
