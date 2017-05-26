#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <glfw3.h>

#include <math.h>

#include "tube.h"

#define PI 3.14159265

GLushort* indices;
GLfloat* verts;

tube::tube(){
    GLushort indices[circle_verts * (length-1) * 2 * 3];
    GLfloat verts[circle_verts * length * 3];
    for(int z = 0; z < length; z++){
        for(int a = 0; a < circle_verts; a++){

            int vert_index = (z * circle_verts + a) * 3;

            verts[vert_index] = sin(2 * a * PI / circle_verts);
            verts[vert_index+1] = cos(2 * a * PI / circle_verts);
            verts[vert_index+2] = z;

            if(z < length-10){
                int index_index = (z * circle_verts + a) * 3 * 2;
                indices[index_index] = z * circle_verts + a;
                indices[index_index+1] = (z+1) * circle_verts + (a+1)%length;
                indices[index_index+2] = (z+1) * circle_verts + a;

                indices[index_index+3] = z * circle_verts + a;
                indices[index_index+4] = z * circle_verts + (a+1)%length;
                indices[index_index+5] = (z+1) * circle_verts + (a+1)%length;

            }
        }
    }
    this->indices = indices;
    this->verts = verts;


}
