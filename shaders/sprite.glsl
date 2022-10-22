#ifdef VERTEX_SHADER
layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_uv;

out vec2 uv;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(in_position, 0, 1);

    uv = in_uv;
}
#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
in vec2 uv;

out vec4 out_color;

uniform sampler2D image;
uniform vec4 color;

void main()
{
    out_color = texelFetch(image, ivec2(uv), 0) * color;
}
#endif // FRAGMENT_SHADER