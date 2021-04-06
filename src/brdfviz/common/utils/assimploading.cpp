/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "assimploading.h"

void loadModel(const ObjLoaderSettings &loaderSettings,
               std::vector<AssimpLoadedModel> &models) {
  models.clear();
  std::vector<std::shared_ptr<Material>> materials;
  
  double currentTime = glfwGetTime();
  Assimp::Importer importer;
  unsigned int importOptions = aiProcess_Triangulate
                               | aiProcess_OptimizeMeshes
                               | aiProcess_JoinIdenticalVertices;
  //importOptions = 0;
  const aiScene *scene = importer.ReadFile(loaderSettings.path, importOptions);
  double lastTime = glfwGetTime();
  spdlog::info("[MODEL LOADER] aiScene loaded in {}s", lastTime - currentTime);
  
  if (scene) {
    std::string directory = loaderSettings.path.substr(0, loaderSettings.path.find_last_of('/') + 1);
    spdlog::info("[MODEL LOADER] Directory: {}\n\tscene->mNumMeshes = {} \n\tscene->mNumMaterials = {}",
                 directory, scene->mNumMeshes, scene->mNumMaterials);
    
    if (loaderSettings.loadMaterial) {
      //Materials
      for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        const aiMaterial *assimpMaterial = scene->mMaterials[i];
        aiString name;
        assimpMaterial->Get(AI_MATKEY_NAME, name);
        spdlog::info("[MODEL LOADER] Material [{}] name {}", i, name.C_Str());
        
        aiColor4D d;
        float f;
        std::shared_ptr<Material> objectMaterial = std::make_shared<Material>();
        
        objectMaterial->set_name(name.C_Str());
        
        
        //Colors
        if (AI_SUCCESS == aiGetMaterialColor(assimpMaterial, AI_MATKEY_COLOR_DIFFUSE, &d)) {
          objectMaterial->diffuse_ = Color3f({d.r, d.g, d.b});
        }
        
        if (AI_SUCCESS == aiGetMaterialColor(assimpMaterial, AI_MATKEY_COLOR_AMBIENT, &d)) {
          objectMaterial->ambient_ = Color3f({d.r, d.g, d.b});
        }
        
        if (AI_SUCCESS == aiGetMaterialColor(assimpMaterial, AI_MATKEY_COLOR_SPECULAR, &d)) {
          objectMaterial->specular_ = Color3f({d.r, d.g, d.b});
        }
        
        if (AI_SUCCESS == aiGetMaterialColor(assimpMaterial, AI_MATKEY_COLOR_EMISSIVE, &d)) {
          objectMaterial->emission_ = Color3f({d.r, d.g, d.b});
        }
        
        if (AI_SUCCESS == aiGetMaterialFloat(assimpMaterial, AI_MATKEY_SHININESS, &f)) {
          objectMaterial->shininess = f;
        }
        
        if (AI_SUCCESS == aiGetMaterialFloat(assimpMaterial, AI_MATKEY_REFLECTIVITY, &f)) {
          objectMaterial->reflectivity = f;
        }
        
        if (AI_SUCCESS == aiGetMaterialFloat(assimpMaterial, AI_MATKEY_REFRACTI, &f)) {
          objectMaterial->ior = f;
        }
//        if (AI_SUCCESS == aiGetMaterialFloat(assimpMaterial, AI_MATKEY_ROUGHNESS, &f)) {
//          objectMaterial->roughness_ = f;
//        }
        
        
        //Textures
        #pragma region "TODO textures"
//          if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
//            Texture3f *texture = getTexture(aiTextureType_DIFFUSE, TextureSlot::diffuseSlot, directory, assimpMaterial);
//            objectMaterial->set_texture(static_cast<int>(TextureSlot::diffuseSlot), texture);
//          }
//
//          if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {
//            Texture3f *texture = getTexture(aiTextureType_SPECULAR, TextureSlot::specularSlot, directory, assimpMaterial);
//            objectMaterial->set_texture(static_cast<int>(TextureSlot::specularSlot), texture);
//          }
//
//          if (assimpMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0) {
//            Texture3f *texture = getTexture(aiTextureType_HEIGHT, TextureSlot::normalSlot, directory, assimpMaterial);
//            objectMaterial->set_texture(static_cast<int>(TextureSlot::normalSlot), texture);
//          }
//
//          if (assimpMaterial->GetTextureCount(aiTextureType_OPACITY) > 0) {
//            Texture3f *texture = getTexture(aiTextureType_OPACITY, TextureSlot::opacitySlot, directory, assimpMaterial);
//            objectMaterial->set_texture(static_cast<int>(TextureSlot::opacitySlot), texture);
//          }
//
//          if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0) {
//            Texture3f *texture = getTexture(aiTextureType_DIFFUSE_ROUGHNESS, TextureSlot::roughnessSlot, directory, assimpMaterial);
//            objectMaterial->set_texture(static_cast<int>(TextureSlot::roughnessSlot), texture);
//          }
//
//          if (assimpMaterial->GetTextureCount(aiTextureType_METALNESS) > 0) {
//            Texture3f *texture = getTexture(aiTextureType_METALNESS, TextureSlot::metalicnessSlot, directory, assimpMaterial);
//            objectMaterial->set_texture(static_cast<int>(TextureSlot::metalicnessSlot), texture);
//          }
        #pragma endregion
        
        materials.emplace_back(objectMaterial);
      }
    }
    
    //Objects
    for (unsigned int j = 0; j < scene->mNumMeshes; j++) {
      aiMesh *mesh = scene->mMeshes[j];

//      std::vector<Vertex> vertices;
      AssimpLoadedModel loadedModel;
      for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
//        Vertex v;
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texcoords;
        
        if (mesh->HasPositions()) {
          position.x = mesh->mVertices[i].x;
          
          if (loaderSettings.flip_yz) {
            position.z = mesh->mVertices[i].y;
            position.y = mesh->mVertices[i].z;
          } else {
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;
          }

//          loadedModel.positions.emplace_back(position);
        }
        if (mesh->HasNormals()) {
          normal.x = mesh->mNormals[i].x;
          
          if (loaderSettings.flip_yz) {
            normal.y = mesh->mNormals[i].z;
            normal.z = mesh->mNormals[i].y;
          } else {
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
          }
//          loadedModel.normals.emplace_back(normal);
        }
        if (mesh->HasTextureCoords(0)) {
          texcoords.x = mesh->mTextureCoords[0][i].x;
          texcoords.y = mesh->mTextureCoords[0][i].y;
//          loadedModel.texcoords.emplace_back(texcoords);
        }
        if (mesh->HasTangentsAndBitangents()) {
          tangent.x = mesh->mTangents[i].x;
          if (loaderSettings.flip_yz) {
            tangent.y = mesh->mTangents[i].z;
            tangent.z = mesh->mTangents[i].y;
          } else {
            tangent.y = mesh->mTangents[i].y;
            tangent.z = mesh->mTangents[i].z;
          }
//          loadedModel.tangents.emplace_back(tangent);
        }
        loadedModel.vertices.emplace_back(Vertex(position, normal, texcoords, tangent));
      }
      
      if (mesh->HasFaces()) {
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
          glm::vec<3, unsigned int> t;
          t.x = mesh->mFaces[i].mIndices[0];
          t.y = mesh->mFaces[i].mIndices[1];
          t.z = mesh->mFaces[i].mIndices[2];
          loadedModel.indices.emplace_back(t);
        }
      }
      
      loadedModel.material = loaderSettings.loadMaterial ? materials.at(mesh->mMaterialIndex) : nullptr;
      
      models.emplace_back(loadedModel);
    }
  } else {
    throw std::runtime_error("[MODEL LOADER] Scene not loaded");
  }
  
}
