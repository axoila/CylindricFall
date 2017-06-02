#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <glfw3.h>

#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include <playground/init.h>

#include "ship.h"

ship::ship(){
    programID = LoadShaders("solid_v.glsl", "solid_f.glsl" );
    //programID = LoadShaders( "solid_v.glsl", "trippy_f.glsl" );
    MatrixID = glGetUniformLocation(programID, "MVP");
    colorID = glGetUniformLocation(programID, "solid_color");
    ViewMatrixID = glGetUniformLocation(programID, "V");
    ModelMatrixID = glGetUniformLocation(programID, "M");
    TextureID = glGetUniformLocation(programID, "matcap");

    // Load the texture
    Texture = loadBMP_custom("matcap.bmp");

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Read our .obj file
    bool res = loadOBJ("ship.obj", vertices, uvs, normals);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

//    glGenBuffers(1, &uvbuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

//    glGenBuffers(1, &elementbuffer);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
}

ship::~ship(){
    // Cleanup VBO and shader
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    glDeleteBuffers(1, &vertexbuffer);
//    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);

}

void ship::draw(mat4 VP, float progress){
    // Use our shader
    glUseProgram(programID);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    mat4 MVP;
    mat4 M;
    mat4 V;
    make_mvp(MVP, M, V, vec3(0, 0, progress),
             vec3(0, 0, progress+1),
             vec3(pos.x, pos.y, progress+5),
             vec3(pos.x - goal.x, pos.y - goal.y, 0),
             vec3(.1, .1, -.1));
    M = glm::inverseTranspose(M);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(TextureID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        1,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_TRUE,            // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void ship::tick(GLFWwindow* window, float delta){
    vec2 input;
    if(glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP))
        input.y += 1;
    if(glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN))
        input.y -= 1;
    if(glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT))
        input.x += 1;
    if(glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT))
        input.x -= 1;

    goal += input * delta;
    if(glm::length(goal) > radius) goal /= length(goal) / radius;
    pos = glm::mix(pos, goal, delta);
}
