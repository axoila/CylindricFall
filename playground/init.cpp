#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <algorithm>

#include <sstream>
#include <vector>
#include <iterator>

#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <common/shader.hpp>

#include "init.h"

GLFWwindow* init_glfw(){
    GLFWwindow* window;

    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return NULL;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // antialiasing
    //glfwWindowHint(GLFW_RESIZABLE,GL_FALSE); //resizable
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //set OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //use new opengl!

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return NULL;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    return window;
}

bool make_mvp(mat4 &MVP, mat4 &M, mat4 &V, vec3 cam_pos, vec3 look_at, vec3 model_pos, vec3 model_lookat, vec3 scale){
    //MVP matrix
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 P = glm::perspective(glm::radians(30.0f), 4.0f/3.0f, 0.1f, 50.0f);
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    V = glm::lookAt(
        cam_pos, // Camera location
        look_at, // camera lok target
        glm::vec3(0,1,0)  // camera up
    );

    // Model matrix : an identity matrix (model will be at the origin)
    M = glm::translate(mat4(1.0), model_pos) *
            glm::rotate(mat4(1.0), model_lookat.x, vec3(0, -1, 0)) *
            glm::rotate(mat4(1.0), model_lookat.y, vec3(1, 0, 0)) *
            glm::scale(mat4(1.0), scale);
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = P * V * M; // Remember, matrix multiplication is the other way around

    V = glm::lookAt(
                glm::vec3(0, 0, 0), // Camera location
                look_at - cam_pos, // camera look target
                glm::vec3(0,1,0)  // camera up
    );
    M =     glm::rotate(mat4(1.0), model_lookat.x, vec3(0, -1, 0)) *
            glm::rotate(mat4(1.0), model_lookat.y, vec3(1, 0, 0));
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

bool loadOBJ(const char* file_path, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals){
    std::ifstream ObjStream(file_path, std::ios::in);
    if(!ObjStream.is_open()){
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
        getchar();
        return 0;
    }

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::string Line = "";
    while(std::getline(ObjStream, Line)){
        std::vector<std::string> args = split(Line, ' ');
        if(args[0] == "v"){
            temp_vertices.push_back(vec3(strtod(&(args[1][0]), NULL),strtod(&(args[2][0]), NULL),strtod(&(args[3][0]), NULL)));
        } else
        if(args[0] == "vn"){
            temp_normals.push_back(vec3(strtod(&(args[1][0]), NULL),strtod(&(args[2][0]), NULL),strtod(&(args[3][0]), NULL)));
        } else
        if(args[0] == "vt"){
            temp_uvs.push_back(vec2(strtod(&(args[1][0]), NULL),strtod(&(args[2][0]), NULL)));
        } else
        if(args[0] == "f"){
            for(int i=1;i<4;i++){
                std::vector<std::string> vun = split(args[i], '/');
                vertexIndices.push_back(strtol(&(vun[0][0]), NULL, 0));
                uvIndices.push_back(strtol(&(vun[1][0]), NULL, 0));
                normalIndices.push_back(strtol(&(vun[2][0]), NULL, 0));
            }
        }
    }
    ObjStream.close();

    for(int i=0;i<vertexIndices.size();i++){
        vertices.push_back(temp_vertices[vertexIndices[i]]);
    }
    for(int i=0;i<normalIndices.size();i++){
        normals.push_back(temp_normals[normalIndices[i]]);
    }

    return 1;
}
