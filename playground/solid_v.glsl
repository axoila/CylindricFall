#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertex_normals;

// Output data ; will be interpolated for each fragment.
out vec3 Position_worldspace;
out vec4 screen_normals;
flat out vec3 vertex_color;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main(void)
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    Position_worldspace = gl_Position.xyz;

    screen_normals = ( V * M * vec4(vertex_normals,0));
    //Only correct if ModelMatrix does not scale the model !
    //Use its inverse transpose if not.;
    vertex_color = vec3(1, 0, 0);
}
