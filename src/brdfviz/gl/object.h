/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef OBJECT_H
#define OBJECT_H


//#include "VertexBufferObject.h"
//#include "Shader.h"
//#include "ObjectTransformation.h"
//#include "texture.h"
//#include "Material.h"

class Material;

class VertexBufferObject;

class Shader;

class ObjectTransformation;

class Object {
public:
//  Object(VertexBufferObject *innerObject, Shader *shader);
//
//  Object(VertexBufferObject *innerObject, Shader *shader, ObjectTransformation *transformation = nullptr);
//
  Object(VertexBufferObject *innerObject, Shader *shader, ObjectTransformation *transformation = nullptr, Material *material = nullptr);

//  Object(Shader *shader); //implicitne vytvori kouli
  ~Object();
  
  //bool hasShader();
  void draw(const bool geometry = false);
  
  void setShader(Shader *shader);
  
  void setVisible(bool visible);
  
  bool isVisible();
  
  bool hasMaterial();
  
  void addMaterial(Material *material);
  
  void setPosition(glm::vec3 position);
  
  int getID();
  
  void addName(std::string);
  
  static Object *getObjectByID(int ID);

//private:
  bool visible;
  std::string *objectName;
  static std::map<int, Object *> objects;
  
  Material *material;
  bool hasMtl;
  
  VertexBufferObject *innerObject;
  int ID;
  static int IDcounter;
  Shader *shader;
  ObjectTransformation *transformation;
};

#endif //OBJECT_H
