#pragma once
#include"includes.h"

class Model 
{
public:
    Model(const char* path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader);
    void DrawInstance(Shader& shader, glm::mat4 *models, unsigned int modelNum);
private:
    /*  模型数据  */
    vector<Mesh> meshes;
    string directory;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);
};