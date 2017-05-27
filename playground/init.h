#ifndef INIT_H
#define INIT_H

GLFWwindow* init_glfw();
bool make_mvp(mat4 &MVP,
              mat4 &V,
              mat4 &P,
              vec3 cam_pos = vec3(0, 0, 0),
              vec3 look_at = vec3(0, 0, 1),
              vec3 model_pos = vec3(0, 0, 0),
              glm::vec3 model_lookat = vec3(0, 0, -1),
              vec3 scale = vec3(1, 1, 1));
bool loadOBJ(const char* file_path,
             std::vector<glm::vec3> &vertices,
             std::vector<glm::vec3> &normals);

#endif // INIT_H
