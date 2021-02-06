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
  Object(const std::shared_ptr<VertexBufferObject> &innerObject, const std::shared_ptr<Shader> &shader,
         const std::shared_ptr<ObjectTransformation> &transformation = nullptr,
         const std::shared_ptr<Material> &material = nullptr);
  
  virtual //bool hasShader();
  void draw(const bool geometry = false);
  
  void setShader(const std::shared_ptr<Shader> &shader);
  
  void setVisible(bool visible);
  
  bool isVisible();
  
  bool hasMaterial();
  
  void addMaterial(const std::shared_ptr<Material> &material);
  
  void setPosition(glm::vec3 position);
  
  int getID();
  
  void addName(const std::string &);
  
  static Object *getObjectByID(int ID);

//private:
  bool visible;
  std::string objectName;
  static std::map<int, Object *> objects;
  
  std::shared_ptr<Material> material;
  bool hasMtl;
  
  std::shared_ptr<VertexBufferObject> innerObject;
  int ID;
  static int IDcounter;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<ObjectTransformation> transformation;
};

#endif //OBJECT_H
