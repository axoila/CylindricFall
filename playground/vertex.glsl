#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec3 Position_worldspace;
flat out vec3 vertex_color;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform float time;

void main(){

        // Output position of the vertex, in clip space : MVP * position
        gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

        float random = mod((gl_Position.x + gl_Position.y + gl_Position.z) * 10000.0, 1) *2*3.1415926;

        vertex_color = vec3(sin(time+random)+1, cos(time+random)+1, -sin(time+random)+1) * 0.5;
}

