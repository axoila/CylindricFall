#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <glfw3.h>

#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>

#include "tube.h"

#define PI 3.14159265


tube::tube(){
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "trippy_v.glsl", "trippy_f.glsl" );

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    timeID = glGetUniformLocation(programID, "time");

    GLushort indices[circle_verts * (length-1) * 2 * 3];
    GLfloat verts[circle_verts * length * 3];
    for(int z = 0; z < length; z++){
        for(int a = 0; a < circle_verts; a++){

            int vert_index = (z * circle_verts + a) * 3;

            verts[vert_index] = sin(2.0 * a * PI / circle_verts);
            verts[vert_index+1] = cos(2.0 * a * PI / circle_verts);
            verts[vert_index+2] = z;

            if(z < length-1){
                int index_index = (z * circle_verts + a) * 3 * 2;
                indices[index_index+2] = z * circle_verts + ((a)%circle_verts);
                indices[index_index+0] = (z+1) * circle_verts + ((a+1)%circle_verts);
                indices[index_index+1] = (z+1) * circle_verts + ((a)%circle_verts);

                indices[index_index+3] = z * circle_verts + ((a)%circle_verts);
                indices[index_index+4] = z * circle_verts + ((a+1)%circle_verts);
                indices[index_index+5] = (z+1) * circle_verts + ((a+1)%circle_verts);

            }
        }
    }
    this->indices = indices;
    this->verts = verts;

    v_vertices = std::vector<GLfloat>(verts, verts + sizeof verts / sizeof verts[0]);

    v_indices = std::vector<unsigned short>(indices, indices + (length * circle_verts * 3));
    // Load it into a VBO

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, v_vertices.size() * sizeof(GLfloat), &v_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, v_indices.size() * sizeof(unsigned short), &v_indices[0] , GL_STATIC_DRAW);
}

tube::~tube(){
    // Cleanup VBO and shader
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
}

void tube::draw(glm::mat4 VP){
    // Use our shader
    glUseProgram(programID);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &VP[0][0]);
    glUniform1f(timeID, (float)glfwGetTime());

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

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        v_indices.size(),    // count
        GL_UNSIGNED_SHORT, // type
        (void*)0           // element array buffer offset
    );

    glDisableVertexAttribArray(0);
}
