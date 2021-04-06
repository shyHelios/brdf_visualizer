/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef ASSIMPLOADING_H
#define ASSIMPLOADING_H

#include <gl/material.h>
#include <gl/vertex.h>


struct ObjLoaderSettings {
  std::string path;
  bool loadMaterial = true;
  bool flip_yz = true;
//  bool useAssimpMaterialLoader = false;
};

struct AssimpLoadedModel {
  std::vector<Vertex> vertices;
  std::vector<glm::vec<3, unsigned int>> indices;
  
  std::shared_ptr<Material> material;
};

void loadModel(const ObjLoaderSettings &loaderSetings,
               std::vector<AssimpLoadedModel> &models);


#endif //ASSIMPLOADING_H
