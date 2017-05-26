#version 330 core
in vec3 Position_worldspace;
flat in vec3 vertex_color;

out vec3 color;

uniform float time;

void main()
{
    color = vertex_color;

}
