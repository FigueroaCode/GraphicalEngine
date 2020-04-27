#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 texture_transform;
uniform mat4 vertex_transform;
uniform sampler2D myTexture;

out vec4 graph_coord;

void main()
{
    graph_coord = texture_transform * vec4(position, 0, 1);
    //graph_coord.z = (texture2D(myTexture, graph_coord.xy / 2.0 + 0.5).r);
    float t = sqrt(graph_coord.x * graph_coord.x + graph_coord.y * graph_coord.y) * 4;
    graph_coord.z = (1 - t * t) * exp(t * t / -2.0);
    gl_Position = vertex_transform * vec4(position, graph_coord.z, 1);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 graph_coord;

void main()
{
    color = graph_coord / 2.0 + 0.5;
}