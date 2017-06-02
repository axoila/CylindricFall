#version 330 core
float PI = 3.1415926;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec3 Position_worldspace;
flat out vec3 vertex_color;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform float time;

float rand(vec2 seed){
    return fract(sin(dot(seed.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(){
    vec4 cam = (MVP * vec4(0, 0, 0, 1));
    float progress = cam.z;
    float offset = ceil(progress*0.996002)+1;

    // Output position of the vertex, in clip space : MVP * position
    vec4 vertex_pos = vec4(vertexPosition_modelspace + vec3(0, 0, -offset),1);
    gl_Position =  MVP * vertex_pos;
    Position_worldspace = gl_Position.xyz;
    //float random = rand(vertex_pos.xz + vertex_pos.yz) * 2 * 3.1415926;
    float random = sin(asin(mod(vertex_pos.x*1000+vertex_pos.y*1000,1)) * PI * 4000 + vertex_pos.z*0.03);

    vertex_color = hsv2rgb(vec3(time*0.5+random, .8, 1));
}
