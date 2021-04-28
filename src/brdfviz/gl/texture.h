/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <FreeImage.h>
#include <string>
#include "color.h"

FIBITMAP *BitmapFromFile(const char *file_name, int &width, int &height);

// Note that all float images in FreeImage are forced to have a range in <0, 1> after applying build-in conversions!!!
// see https://sourceforge.net/p/freeimage/bugs/259/
FIBITMAP *Custom_FreeImage_ConvertToRGBF(FIBITMAP *dib); // this fix removes clamp from conversion of float images
FIBITMAP *Custom_FreeImage_ConvertToRGBAF(FIBITMAP *dib);  // this fix removes clamp from conversion of float images

/*! \class Texture
\brief A simple templated representation of texture.
Texture3f normal = Texture3f( "../../../data/denoise/normal_100spp.exr");
Texture3f output = Texture3f( width, height );
output.Save( "denoised.exr" );
\author Tomáš Fabián
\version 1.0
\date 2020
*/

template<class T, FREE_IMAGE_TYPE F>
class Texture {
public:
  Texture(const int width, const int height, int slot = 0);
  
  explicit Texture(const std::string &file_name, int slot = 0);
  
  explicit Texture(std::vector<std::string> filenames, int slot = 0);
  
  T pixel(const int x, const int y) const;
  
  T texel(const float u, const float v) const;
  
  int width() const;
  
  int height() const;
  
  T *data();
  
  FIBITMAP *Convert(FIBITMAP *dib);
  
  static Texture Load(const std::string &file_name) {
    throw "Load method is defined only for particular Texture types";
  }
  
  void Save(const std::string &file_name) const;
  
  unsigned int getTextureId() const;
  
  unsigned int getTextureUnit() const;
  
  void setTextureUnit(unsigned int textureUnit);
  
  unsigned int getMaxLevel() const;
  
  std::vector<T> &getData();

private:
  std::vector<T> data_;
  
  int width_{0};
  int height_{0};
  unsigned int max_level{8};
  unsigned int textureID_{0};
  
  unsigned int textureUnit_{0};
  
  void createGLTexture(int slot = 0);
  
  void createGLTexture(int slot, T *dataPtr,
                       int mipmapLevel,
                       bool genTextureId,
                       unsigned int width,
                       unsigned int height);
};


using Texture3f = Texture<Color3f, FIT_RGBF>;
using Texture4f = Texture<Color4f, FIT_RGBAF>;
using Texture3u = Texture<Color3u, FIT_BITMAP>;
using Texture4u = Texture<Color4u, FIT_BITMAP>;

#endif //TEXTURE_H
