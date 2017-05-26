#ifndef INIT_H
#define INIT_H

GLFWwindow* init_glfw();
mat4 make_mvp(vec3 cam_pos = vec3(0, 0, 0),
              vec3 look_at = vec3(0, 0, 1));

#endif // INIT_H
