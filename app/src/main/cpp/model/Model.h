//
// Created by Administrator on 2022/8/4.
//

#ifndef TESTOPENGL_MODEL_H
#define TESTOPENGL_MODEL_H

#include <vector>
#include <glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <opencv2/opencv.hpp>
#include "Mesh.h"
#include "../util/LogUtil.h"

using namespace std;

class Model {

public:
    vector<Texture> texturesLoaded;
    vector<Mesh> meshes;
    string directory;
    glm::vec3 maxXyz, minXyz;
    bool gammaCorrection;
    bool hasTexture;

    Model(string const &path, bool gamma = false);

    void Draw(Shader shader);

    bool ContainsTextures();

    void Destroy();

    float GetMaxViewDistance();

    glm::vec3 GetAdjustModelPosVec();

private:
    void LoadModel(string const &path);

    void ProcessNode(aiNode *node, const aiScene *scene);

    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

    vector<Texture> LoadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName);

    unsigned int GetTextureFromFile(const char *path, const string &directory, bool gamma = false);

    void UpdateMaxMinXyz(glm::vec3 pos);
};


#endif //TESTOPENGL_MODEL_H
