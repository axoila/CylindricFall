#ifndef TUBE_H
#define TUBE_H

#define circle_verts 16
#define length 60

class tube
{
public:
    GLfloat* verts;
    GLushort* indices;
    tube();
};

#endif // TUBE_H
