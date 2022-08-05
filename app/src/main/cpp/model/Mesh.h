//
// Created by Administrator on 2022/8/4.
//

#ifndef TESTOPENGL_MESH_H
#define TESTOPENGL_MESH_H

#include <iostream>
#include <string>
#include <glm.hpp>
#include <vector>
#include <GLES3/gl3.h>
#include <stddef.h>
#include "Shader.h"
#include "../util/LogUtil.h"

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int vao;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

    void Draw(Shader shader);

    void Destroy();
private:
    unsigned int vbo, ebo;

    void SetupMesh();
};

#endif //TESTOPENGL_MESH_H
