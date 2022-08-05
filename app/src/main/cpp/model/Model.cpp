//
// Created by Administrator on 2022/8/4.
//

#include "Model.h"

Model::Model(const string &path, bool gamma) {
    this->hasTexture = false;
    this->gammaCorrection = gamma;
    LoadModel(path);
}

void Model::Draw(Shader shader) {
    for (unsigned int i = 0; i < meshes.size(); ++i) {
        meshes[i].Draw(shader);
    }
}

bool Model::ContainsTextures() {
    return hasTexture;
}

void Model::Destroy() {
    for (int i = 0; i < meshes.size(); ++i) {
        meshes[i].Destroy();
    }
}

float Model::GetMaxViewDistance() {
    glm::vec3 vec3 = (abs(minXyz) + abs(maxXyz)) / 2.0f;
    float maxDis = fmax(vec3.x, fmax(vec3.y, vec3.z));
    return maxDis;
}

glm::vec3 Model::GetAdjustModelPosVec() {
    return (minXyz + maxXyz) / 2.0f;
}

void Model::LoadModel(const string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOGCATE("Model::loadModel path=%s, assimpError=%s", path.c_str(), importer.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        if (mesh != nullptr) {
            meshes.push_back(ProcessMesh(mesh, scene));
        }
    }
    DEBUG_LOGCATE();
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        UpdateMaxMinXyz(vector);

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0]) {
            glm::vec2 texCoord;
            texCoord.x = mesh->mTextureCoords[0][i].x;
            texCoord.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = texCoord;
        } else {
            vertex.texCoord = glm::vec2(0.0f, 0.0f);
        }

        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.tangent = vector;

        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.bitangent = vector;

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // diffuse
    vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // specular
    vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // normal
//    vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // height
//    vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
        aiString str;
        material->GetTexture(type, i, &str);
        bool skip = false;
        for (int j = 0; j < texturesLoaded.size(); ++j) {
            if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = GetTextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::GetTextureFromFile(const char *path, const string &directory, bool gamma) {
    string filename = string(path);
    filename = directory + "/" + filename;
    unsigned int textureId;
    glGenTextures(1, &textureId);

    cv::Mat textureImage = cv::imread(filename);
    if (!textureImage.empty()) {
        hasTexture = true;
        // opencv reads textures in BGR format, change to RGB for GL
        cv::cvtColor(textureImage, textureImage, CV_BGR2RGB);

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.cols, textureImage.rows, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, textureImage.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }

    return textureId;
}

void Model::UpdateMaxMinXyz(glm::vec3 pos) {
    maxXyz.x = pos.x > maxXyz.x ? pos.x : maxXyz.x;
    maxXyz.y = pos.y > maxXyz.y ? pos.y : maxXyz.y;
    maxXyz.z = pos.z > maxXyz.z ? pos.z : maxXyz.z;

    minXyz.x = pos.x < minXyz.x ? pos.x : minXyz.x;
    minXyz.y = pos.y < minXyz.y ? pos.y : minXyz.y;
    minXyz.z = pos.z < minXyz.z ? pos.z : minXyz.z;
}
