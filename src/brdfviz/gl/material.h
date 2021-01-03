/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "texture.h"

/*! \def NO_TEXTURES
\brief Maximální počet textur přiřazených materiálu.
*/
#define NO_TEXTURES 9

/*! \def IOR_AIR
\brief Index lomu vzduchu za normálního tlaku.
*/
#define IOR_AIR 1.000293f

/*! \def IOR_AIR
\brief Index lomu vody.
*/
#define IOR_WATER 1.33f

/*! \def IOR_GLASS
\brief Index lomu skla.
*/
#define IOR_GLASS 1.5f

/* types of shaders */
enum class ShaderEnum : char {
  NORMAL = 1, LAMBERT = 2, PHONG = 3, GLASS = 4, PBR = 5, MIRROR = 6, TS = 7, CT = 8
};

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

/*! \class Material
\brief A simple material.
\author Tomáš Fabián
\version 0.9
\date 2011-2018
*/
class Material {
public:
  //! Implicitní konstruktor.
  /*!
  Inicializuje všechny složky materiálu na výchozí matně šedý materiál.
  */
  Material();
  
  //! Specializovaný konstruktor.
  /*!
  Inicializuje materiál podle zadaných hodnot parametrů.
  \param name název materiálu.
  \param ambient barva prostředí.
  \param diffuse barva rozptylu.
  \param specular barva odrazu.
  \param emission barva emise.
  \param shininess lesklost.
  \param ior index lomu.
  \param shader shader to be used.
  \param textures pole ukazatelů na textury.
  \param no_textures délka pole \a textures. Maximálně \a NO_TEXTURES - 1.
  */
  Material(const std::string &name,
           const Color3f &ambient = Color3f{{0.1f, 0.1f, 0.1f}},
           const Color3f &diffuse = Color3f{{0.5f, 0.5f, 0.5f}},
           const Color3f &specular = Color3f{{0.6f, 0.6f, 0.6f}},
           const Color3f &emission = Color3f{{0.0f, 0.0f, 0.0f}},
           const float reflectivity = 0.99,
           const float shininess = 1.0,
           const float ior = -1.0,
           const ShaderEnum shader = ShaderEnum::PHONG,
           Texture3f **textures = nullptr,
           const int no_textures = 0);
  
  
  //! Destruktor.
  /*!
  Uvolní všechny alokované zdroje.
  */
  ~Material();
  
  //void Print();
  
  //! Nastaví název materiálu.
  /*!
  \param name název materiálu.
  */
  void set_name(const char *name);
  
  //! Vrátí název materiálu.
  /*!
  \return Název materiálu.
  */
  std::string name() const;
  
  //! Nastaví texturu.
  /*!
  \param slot číslo slotu, do kterého bude textura přiřazena. Maximálně \a NO_TEXTURES - 1.
  \param texture ukazatel na texturu.
  */
  void set_texture(const int slot, Texture3f *texture);
  
  //! Nastaví texturu.
  /*!
  \param texture ukazatel na texturu.
  */
  void set_texture(Texture3f *texture);
  
  //! Vrátí texturu.
  /*!
  \param slot číslo slotu textury. Maximálně \a NO_TEXTURES - 1.
  \return Ukazatel na zvolenou texturu.
  */
  Texture3f
  *
  
  texture(const int slot) const;
  
  ShaderEnum shader() const;
  
  void set_shader(ShaderEnum shader);
  
  Color3f ambient(const glm::vec2 *tex_coord = nullptr) const;
  
  Color3f diffuse(const glm::vec2 *tex_coord = nullptr) const;
  
  Color3f specular(const glm::vec2 *tex_coord = nullptr) const;
  
  Color3f bump(const glm::vec2 *tex_coord = nullptr) const;
  
  float roughness(const glm::vec2 *tex_coord = nullptr) const;
  
  Color3f emission(const glm::vec2 *tex_coord = nullptr) const;
  
  void set(Material *mtl);
  
  void setName(const std::string &name);

public:
  Color3f ambient_; /*!< RGB barva prostředí \f$\left<0, 1\right>^3\f$. */
  Color3f diffuse_; /*!< RGB barva rozptylu \f$\left<0, 1\right>^3\f$. */
  Color3f specular_; /*!< RGB barva odrazu \f$\left<0, 1\right>^3\f$. */
  Color3f emission_; /*!< RGB barva emise \f$\left<0, 1\right>^3\f$. */
  
  float shininess; /*!< Koeficient lesklosti (\f$\ge 0\f$). Čím je hodnota větší, tím se jeví povrch lesklejší. */
  float roughness_; /*!< Koeficient drsnosti. */
  float metallicness; /*!< Koeficient kovovosti. */
  float reflectivity; /*!< Koeficient odrazivosti. */
  float ior; /*!< Index lomu. */
  
  bool hasTexture;
private:
  Texture3f *textures_[static_cast<int>(TextureSlot::numberOfSlots)]; /*!< Pole ukazatelů na textury. */
  
  std::string name_; /*!< Material name. */
  
  ShaderEnum shader_{ShaderEnum::NORMAL}; /*!< Type of used shader. */
};


#endif //MATERIAL_H
