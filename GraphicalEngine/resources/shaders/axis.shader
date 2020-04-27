#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 transform;

out vec4 v_Color;

void main()
{
    gl_Position = transform * vec4(position.xy, 0, 1);
    v_Color = vec4(position.xy / 2.0 + 0.5, 1, 1);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
}