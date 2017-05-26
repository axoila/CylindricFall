#version 330 core

in vec3 Position_worldspace;
flat in vec3 vertex_color;

out vec3 color;

uniform float time;

void main()
{
    color = mix(vertex_color, vec3(1, 1, 1),
                clamp(Position_worldspace.z/30-0.1,0, 1));

}
