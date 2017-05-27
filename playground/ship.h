#ifndef SHIP_H
#define SHIP_H


class ship
{
public:
    ship();
    void draw(mat4 VP, float progress = 0);
    void tick(GLFWwindow *window, float delta = 0.02f);
    virtual ~ship();
private:
    glm::vec2 pos;
    glm::vec2 goal;

    GLuint programID;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint colorID;
    GLuint TextureID;

    GLuint Texture;

    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    //GLuint elementbuffer;

    //std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
};

#endif // SHIP_H
