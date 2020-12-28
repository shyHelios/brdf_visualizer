/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef UNIFORMLOCATIONS_H
#define UNIFORMLOCATIONS_H

struct UniformLocations {
  int ModelTransform = -1; //Modelova matice
  int ViewTransform = -1; //Pohledova matice
  int ProjectionTransform = -1; //Projekcni matice
  
  int LightCount = -1; //Pocet svetel
  
  int CameraPosition = -1; //Pozice kamery
  
  int MaterialDiffuseColor = -1;
  int MaterialNormalTexture = -1;
  int MaterialDiffuseTexture = -1;
  int MaterialSpecularTexture = -1;
  int MaterialHasDiffuseColor = -1;
  int MaterialHasDiffuseTexture = -1;
  int MaterialHasSpecularTexture = -1;
  int MaterialHasNormalTexture = -1;
  int MaterialShininess = -1;
};


#endif //UNIFORMLOCATIONS_H
