/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "texture.h"

#define NO_TEXTURES 9
#define IOR_AIR 1.000293f
#define IOR_WATER 1.33f
#define IOR_GLASS 1.5f

/* types of shaders */
enum class ShaderEnum : char {
  NORMAL = 1, LAMBERT = 2, PHONG = 3, GLASS = 4, PBR = 5, MIRROR = 6, TS = 7, CT = 8
};

inline bool isShader(int i) {
  return i >= (int) ShaderEnum::NORMAL && i <= (int) ShaderEnum::CT;
}

enum class TextureSlot : int {
  diffuseSlot = 0,
  specularSlot,
  normalSlot,
  opacitySlot,
  roughnessSlot,
  metalicnessSlot,
  integrationSlot,
  irradianceSlot,
  environmentSlot,
  numberOfSlots
};

class Material {
public:
  
  Material();
  
  Material(const std::string &name,
           const Color3f &ambient = Color3f{{0.1f, 0.1f, 0.1f}},
           const Color3f &diffuse = Color3f{{0.5f, 0.5f, 0.5f}},
           const Color3f &specular = Color3f{{0.6f, 0.6f, 0.6f}},
           const Color3f &emission = Color3f{{0.0f, 0.0f, 0.0f}},
           const float reflectivity = 0.99,
           const float shininess = 1.0,
           const float ior = -1.0,
           const ShaderEnum shader = ShaderEnum::PHONG,
           const std::array<std::shared_ptr<Texture3f>, static_cast<int>(TextureSlot::numberOfSlots)> &textures = {nullptr},
           const int no_textures = 0);
  
  
  virtual ~Material() = default;
  
  
  void set_name(const char *name);
  
  
  std::string name() const;
  
  
  void set_texture(const int slot, const std::shared_ptr<Texture3f> &texture);
  
  void set_texture(const TextureSlot slot, const std::shared_ptr<Texture3f> &texture);
  
  
  void set_texture(const std::shared_ptr<Texture3f> &texture);
  
  std::shared_ptr<Texture3f> texture(const TextureSlot slot) const;
  
  std::shared_ptr<Texture3f> texture(const int slot) const;
  
  ShaderEnum shader() const;
  
  void set_shader(ShaderEnum shader);
  
  Color3f ambient(const glm::vec2 *tex_coord = nullptr) const;
  
  Color3f diffuse(const glm::vec2 *tex_coord = nullptr) const;
  
  Color3f specular(const glm::vec2 *tex_coord = nullptr) const;
  
  Color3f bump(const glm::vec2 *tex_coord = nullptr) const;
  
  float roughness(const glm::vec2 *tex_coord = nullptr) const;
  
  Color3f emission(const glm::vec2 *tex_coord = nullptr) const;
  
  void set(const std::shared_ptr<Material> &mtl);
  
  void setName(const std::string &name);

public:
  Color3f ambient_;
  Color3f diffuse_;
  Color3f specular_;
  Color3f emission_;
  
  float shininess;
  float roughness_;
  float metallicness;
  float reflectivity;
  float ior;
  
  bool hasTexture;
private:
  std::array<std::shared_ptr<Texture3f>, static_cast<int>(TextureSlot::numberOfSlots)> textures_;
  
  std::string name_;
  
  ShaderEnum shader_{ShaderEnum::NORMAL};
};


#endif //MATERIAL_H
