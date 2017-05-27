#ifndef TUBE_H
#define TUBE_H

#define circle_verts 32
#define length 100

class tube
{
public:
    GLfloat* verts;
    GLushort* indices;

    std::vector<GLfloat> v_vertices;
    std::vector<unsigned short> v_indices;

    tube();
    void draw(glm::mat4 VP);
    virtual ~tube();

private:
    GLuint programID;
    GLuint MatrixID;
    GLuint timeID;

    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint elementbuffer;
};

#endif // TUBE_H
