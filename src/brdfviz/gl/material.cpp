/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "material.h"

Material::Material() {
  // defaultní materiál
  ambient_ = Color3f{{0.1f, 0.1f, 0.1f}};
  diffuse_ = Color3f{{0.5f, 0.5f, 0.5f}};
  specular_ = Color3f{{0.6f, 0.6f, 0.6f}};
  
  emission_ = Color3f{{0.0f, 0.0f, 0.0f}};
  
  reflectivity = static_cast<float>( 0.99 );
  shininess = 1.0f;
  roughness_ = 1.0f;
  metallicness = 0.0f;
  
  ior = -1.0f;

//  memset(textures_, 0, sizeof(*textures_) * NO_TEXTURES);
  
  for (int i = 0; i < static_cast<int>(TextureSlot::numberOfSlots); i++) {
    textures_.at(i) = nullptr;
  }
  hasTexture = false;
  
  name_ = "default";
  shader_ = ShaderEnum::PHONG;
}

Material::Material(const std::string &name, const Color3f &ambient, const Color3f &diffuse,
                   const Color3f &specular, const Color3f &emission, const float reflectivity,
                   const float shininess, const float ior, const ShaderEnum shader,
                   const std::array<std::shared_ptr<Texture3f>, static_cast<int>(TextureSlot::numberOfSlots)> &textures,
                   const int no_textures) {
  name_ = name;
  
  ambient_ = ambient;
  diffuse_ = diffuse;
  specular_ = specular;
  
  emission_ = emission;
  
  this->reflectivity = reflectivity;
  this->shininess = shininess;
  
  this->ior = ior;
  
  hasTexture = false;
  
  for (int i = 0; i < static_cast<int>(TextureSlot::numberOfSlots); i++) {
    if (textures.at(i) != nullptr) {
      hasTexture = true;
    }
    textures_.at(i) = textures.at(i);
  }
}

void Material::set_name(const char *name) {
  name_ = std::string(name);
}

std::string Material::name() const {
  return name_;
}

void Material::set_texture(const std::shared_ptr<Texture3f> &texture) {
  textures_[texture->getTextureUnit()] = texture;
}

void Material::set_texture(const int slot, const std::shared_ptr<Texture3f> &texture) {
  textures_[slot] = texture;
  texture->setTextureUnit(slot);
}

void Material::set_texture(const TextureSlot slot, const std::shared_ptr<Texture3f> &texture) {
  set_texture(static_cast<int>(slot), texture);
}

std::shared_ptr<Texture3f> Material::texture(const int slot) const {
  return textures_[slot];
}

ShaderEnum Material::shader() const {
  return shader_;
}

void Material::set_shader(ShaderEnum shader) {
  shader_ = shader;
}

Color3f Material::ambient(const glm::vec2 *tex_coord) const {
  return ambient_;
}

Color3f Material::diffuse(const glm::vec2 *tex_coord) const {
  if (tex_coord) {
    std::shared_ptr<Texture3f> texture = textures_[static_cast<int>(TextureSlot::diffuseSlot)];
    
    if (texture) {
      return texture->texel(tex_coord->x, tex_coord->y/*, true*/);
    }
  }
  
  return diffuse_;
}

Color3f Material::specular(const glm::vec2 *tex_coord) const {
  if (tex_coord) {
    std::shared_ptr<Texture3f> texture = textures_[static_cast<int>(TextureSlot::specularSlot)];
    
    if (texture) {
      return texture->texel(tex_coord->x, tex_coord->y/*, true*/);
    }
  }
  
  return specular_;
}

Color3f Material::bump(const glm::vec2 *tex_coord) const {
  if (tex_coord) {
    std::shared_ptr<Texture3f> texture = textures_[static_cast<int>(TextureSlot::normalSlot)];
    
    if (texture) {
      return texture->texel(tex_coord->x, tex_coord->y/*, false*/);
    }
  }
  
  return Color3f{{0.5f, 0.5f, 1.0f}}; // n = ( 0, 0, 1 )
}

float Material::roughness(const glm::vec2 *tex_coord) const {
  
  if (tex_coord) {
    std::shared_ptr<Texture3f> texture = textures_[static_cast<int>(TextureSlot::roughnessSlot)];
    
    if (texture) {
      return texture->texel(tex_coord->x, tex_coord->y/*, false*/).data[0];
    }
  }
  
  return roughness_;
}

Color3f Material::emission(const glm::vec2 *tex_coord) const {
  return emission_;
}

void Material::set(const std::shared_ptr<Material> &mtl) {
  name_ = mtl->name_;
  
  ambient_ = mtl->ambient_;
  diffuse_ = mtl->diffuse_;
  specular_ = mtl->specular_;
  
  emission_ = mtl->emission_;
  
  this->reflectivity = mtl->reflectivity;
  this->shininess = mtl->shininess;
  
  this->ior = mtl->ior;
  
  if (mtl->hasTexture) {
    for (int i = 0; i < NO_TEXTURES; i++) {
      textures_[i] = mtl->textures_[i];
    }
  }
}

void Material::setName(const std::string &name) {
  name_ = name;
}

std::shared_ptr<Texture3f> Material::texture(const TextureSlot slot) const {
  return texture(static_cast<int>(slot));
}

