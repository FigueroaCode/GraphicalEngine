#shader vertex
#version 330 core

layout(location = 0) in float x_coord;

uniform float offset_x;
uniform float scale_x;
uniform sampler2D yTexture;

out vec4 v_Color;

void main()
{
    float x = (x_coord + offset_x) * scale_x;
    float y = (texture2D(yTexture, vec2(x / 10.24 / 2.0 + 0.5, 0)).r - 0.5) * 2.0;

    gl_Position = vec4(x_coord, y, 0.0, 1.0);
    v_Color = vec4(x / 2.0 + 0.5, y / 2.0 + 0.5, 1, 1);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
}