/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "texture.h"

FIBITMAP *BitmapFromFile(const char *file_name, int &width, int &height) {
  // image format
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  // pointer to the image, once loaded
  FIBITMAP *dib = nullptr;
  // pointer to the image data
  //BYTE * bits = nullptr;
  
  // check the file signature and deduce its format
  fif = FreeImage_GetFileType(file_name, 0);
  // if still unknown, try to guess the file format from the file extension
  if (fif == FIF_UNKNOWN) {
    fif = FreeImage_GetFIFFromFilename(file_name);
  }
  // if known
  if (fif != FIF_UNKNOWN) {
    // check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif)) {
      dib = FreeImage_Load(fif, file_name);
    }
    // if the image loaded
    if (dib) {
      // get the image width and height
      width = int(FreeImage_GetWidth(dib));
      height = int(FreeImage_GetHeight(dib));
      
      // if each of these is ok
      if ((width == 0) || (height == 0)) {
        FreeImage_Unload(dib);
        dib = nullptr;
      }
    }
  }
  
  return dib;
}

FIBITMAP *Custom_FreeImage_ConvertToRGBF(FIBITMAP *dib) {
  FIBITMAP *src = nullptr;
  FIBITMAP *dst = nullptr;
  
  if (!FreeImage_HasPixels(dib)) return nullptr;
  
  const FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(dib);
  
  // check for allowed conversions
  switch (src_type) {
    case FIT_BITMAP: {
      // allow conversion from 24- and 32-bit
      const FREE_IMAGE_COLOR_TYPE color_type = FreeImage_GetColorType(dib);
      if ((color_type != FIC_RGB) && (color_type != FIC_RGBALPHA)) {
        src = FreeImage_ConvertTo24Bits(dib);
        if (!src) return nullptr;
      } else {
        src = dib;
      }
      break;
    }
    case FIT_UINT16:
      // allow conversion from 16-bit
      src = dib;
      break;
    case FIT_RGB16:
      // allow conversion from 48-bit RGB
      src = dib;
      break;
    case FIT_RGBA16:
      // allow conversion from 64-bit RGBA (ignore the alpha channel)
      src = dib;
      break;
    case FIT_FLOAT:
      // allow conversion from 32-bit float
      src = dib;
      break;
    case FIT_RGBAF:
      // allow conversion from 128-bit RGBAF
      src = dib;
      break;
    case FIT_RGBF:
      // RGBF type : clone the src
      return FreeImage_Clone(dib);
      break;
    default: return nullptr;
  }
  
  // allocate dst image
  
  const unsigned width = FreeImage_GetWidth(src);
  const unsigned height = FreeImage_GetHeight(src);
  
  dst = FreeImage_AllocateT(FIT_RGBF, width, height);
  if (!dst) {
    if (src != dib) {
      FreeImage_Unload(src);
    }
    return nullptr;
  }
  
  // copy metadata from src to dst
  FreeImage_CloneMetadata(dst, src);
  
  // convert from src type to RGBF
  
  const unsigned src_pitch = FreeImage_GetPitch(src);
  const unsigned dst_pitch = FreeImage_GetPitch(dst);
  
  switch (src_type) {
    case FIT_BITMAP: {
      // calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
      const unsigned bytespp = FreeImage_GetLine(src) / FreeImage_GetWidth(src);
      
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const BYTE *src_pixel = (BYTE *) src_bits;
        FIRGBF *dst_pixel = (FIRGBF *) dst_bits;
        for (unsigned x = 0; x < width; x++) {
          // convert and scale to the range [0..1]
          dst_pixel->red = (float) (src_pixel[FI_RGBA_RED]) / 255.0F;
          dst_pixel->green = (float) (src_pixel[FI_RGBA_GREEN]) / 255.0F;
          dst_pixel->blue = (float) (src_pixel[FI_RGBA_BLUE]) / 255.0F;
          
          src_pixel += bytespp;
          dst_pixel++;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_UINT16: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const WORD *src_pixel = (WORD *) src_bits;
        FIRGBF *dst_pixel = (FIRGBF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert and scale to the range [0..1]
          const float dst_value = (float) src_pixel[x] / 65535.0F;
          dst_pixel[x].red = dst_value;
          dst_pixel[x].green = dst_value;
          dst_pixel[x].blue = dst_value;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_RGB16: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const FIRGB16 *src_pixel = (FIRGB16 *) src_bits;
        FIRGBF *dst_pixel = (FIRGBF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert and scale to the range [0..1]
          dst_pixel[x].red = (float) (src_pixel[x].red) / 65535.0F;
          dst_pixel[x].green = (float) (src_pixel[x].green) / 65535.0F;
          dst_pixel[x].blue = (float) (src_pixel[x].blue) / 65535.0F;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_RGBA16: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const FIRGBA16 *src_pixel = (FIRGBA16 *) src_bits;
        FIRGBF *dst_pixel = (FIRGBF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert and scale to the range [0..1]
          dst_pixel[x].red = (float) (src_pixel[x].red) / 65535.0F;
          dst_pixel[x].green = (float) (src_pixel[x].green) / 65535.0F;
          dst_pixel[x].blue = (float) (src_pixel[x].blue) / 65535.0F;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_FLOAT: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const float *src_pixel = (float *) src_bits;
        FIRGBF *dst_pixel = (FIRGBF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert by copying greyscale channel to each R, G, B channels
          // NOT assume float values are in [0..1] !!!
          const float value = src_pixel[x];
          dst_pixel[x].red = value;
          dst_pixel[x].green = value;
          dst_pixel[x].blue = value;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_RGBAF: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const FIRGBAF *src_pixel = (FIRGBAF *) src_bits;
        FIRGBF *dst_pixel = (FIRGBF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert and skip alpha channel
          dst_pixel[x].red = src_pixel[x].red;
          dst_pixel[x].green = src_pixel[x].green;
          dst_pixel[x].blue = src_pixel[x].blue;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
  }
  
  if (src != dib) {
    FreeImage_Unload(src);
  }
  
  return dst;
}

FIBITMAP *Custom_FreeImage_ConvertToRGBAF(FIBITMAP *dib) {
  FIBITMAP *src = nullptr;
  FIBITMAP *dst = nullptr;
  
  if (!FreeImage_HasPixels(dib)) return nullptr;
  
  const FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(dib);
  
  // check for allowed conversions
  switch (src_type) {
    case FIT_BITMAP: {
      // allow conversion from 32-bit
      const FREE_IMAGE_COLOR_TYPE color_type = FreeImage_GetColorType(dib);
      if (color_type != FIC_RGBALPHA) {
        src = FreeImage_ConvertTo32Bits(dib);
        if (!src) return nullptr;
      } else {
        src = dib;
      }
      break;
    }
    case FIT_UINT16:
      // allow conversion from 16-bit
      src = dib;
      break;
    case FIT_RGB16:
      // allow conversion from 48-bit RGB
      src = dib;
      break;
    case FIT_RGBA16:
      // allow conversion from 64-bit RGBA
      src = dib;
      break;
    case FIT_FLOAT:
      // allow conversion from 32-bit float
      src = dib;
      break;
    case FIT_RGBF:
      // allow conversion from 96-bit RGBF
      src = dib;
      break;
    case FIT_RGBAF:
      // RGBAF type : clone the src
      return FreeImage_Clone(dib);
      break;
    default: return nullptr;
  }
  
  // allocate dst image
  
  const unsigned width = FreeImage_GetWidth(src);
  const unsigned height = FreeImage_GetHeight(src);
  
  dst = FreeImage_AllocateT(FIT_RGBAF, width, height);
  if (!dst) {
    if (src != dib) {
      FreeImage_Unload(src);
    }
    return nullptr;
  }
  
  // copy metadata from src to dst
  FreeImage_CloneMetadata(dst, src);
  
  // convert from src type to RGBAF
  
  const unsigned src_pitch = FreeImage_GetPitch(src);
  const unsigned dst_pitch = FreeImage_GetPitch(dst);
  
  switch (src_type) {
    case FIT_BITMAP: {
      // calculate the number of bytes per pixel (4 for 32-bit)
      const unsigned bytespp = FreeImage_GetLine(src) / FreeImage_GetWidth(src);
      
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const BYTE *src_pixel = (BYTE *) src_bits;
        FIRGBAF *dst_pixel = (FIRGBAF *) dst_bits;
        for (unsigned x = 0; x < width; x++) {
          // convert and scale to the range [0..1]
          dst_pixel->red = (float) (src_pixel[FI_RGBA_RED]) / 255.0F;
          dst_pixel->green = (float) (src_pixel[FI_RGBA_GREEN]) / 255.0F;
          dst_pixel->blue = (float) (src_pixel[FI_RGBA_BLUE]) / 255.0F;
          dst_pixel->alpha = (float) (src_pixel[FI_RGBA_ALPHA]) / 255.0F;
          
          src_pixel += bytespp;
          dst_pixel++;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_UINT16: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const WORD *src_pixel = (WORD *) src_bits;
        FIRGBAF *dst_pixel = (FIRGBAF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert and scale to the range [0..1]
          const float dst_value = (float) src_pixel[x] / 65535.0F;
          dst_pixel[x].red = dst_value;
          dst_pixel[x].green = dst_value;
          dst_pixel[x].blue = dst_value;
          dst_pixel[x].alpha = 1.0F;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_RGB16: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const FIRGB16 *src_pixel = (FIRGB16 *) src_bits;
        FIRGBAF *dst_pixel = (FIRGBAF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert and scale to the range [0..1]
          dst_pixel[x].red = (float) (src_pixel[x].red) / 65535.0F;
          dst_pixel[x].green = (float) (src_pixel[x].green) / 65535.0F;
          dst_pixel[x].blue = (float) (src_pixel[x].blue) / 65535.0F;
          dst_pixel[x].alpha = 1.0F;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_RGBA16: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const FIRGBA16 *src_pixel = (FIRGBA16 *) src_bits;
        FIRGBAF *dst_pixel = (FIRGBAF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert and scale to the range [0..1]
          dst_pixel[x].red = (float) (src_pixel[x].red) / 65535.0F;
          dst_pixel[x].green = (float) (src_pixel[x].green) / 65535.0F;
          dst_pixel[x].blue = (float) (src_pixel[x].blue) / 65535.0F;
          dst_pixel[x].alpha = (float) (src_pixel[x].alpha) / 65535.0F;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_FLOAT: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const float *src_pixel = (float *) src_bits;
        FIRGBAF *dst_pixel = (FIRGBAF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert by copying greyscale channel to each R, G, B channels
          // NOT assume float values are in [0..1] !!!
          const float value = src_pixel[x];
          dst_pixel[x].red = value;
          dst_pixel[x].green = value;
          dst_pixel[x].blue = value;
          dst_pixel[x].alpha = 1.0F;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
    
    case FIT_RGBF: {
      const BYTE *src_bits = (BYTE *) FreeImage_GetBits(src);
      BYTE *dst_bits = (BYTE *) FreeImage_GetBits(dst);
      
      for (unsigned y = 0; y < height; y++) {
        const FIRGBF *src_pixel = (FIRGBF *) src_bits;
        FIRGBAF *dst_pixel = (FIRGBAF *) dst_bits;
        
        for (unsigned x = 0; x < width; x++) {
          // convert pixels directly, while adding a "dummy" alpha of 1.0
          dst_pixel[x].red = src_pixel[x].red;
          dst_pixel[x].green = src_pixel[x].green;
          dst_pixel[x].blue = src_pixel[x].blue;
          dst_pixel[x].alpha = 1.0F;
        }
        src_bits += src_pitch;
        dst_bits += dst_pitch;
      }
    }
      break;
  }
  
  if (src != dib) {
    FreeImage_Unload(src);
  }
  
  return dst;
}

template<class T, FREE_IMAGE_TYPE F>
Texture<T, F>::Texture(const int width, const int height, int slot) {
  assert(width > 0 && height > 0);
  textureUnit_ = GL_TEXTURE0 + slot;
  
  width_ = width;
  height_ = height;
  
  data_.resize(size_t(width) * size_t(height));
  createGLTexture(slot);
  glBindTexture(GL_TEXTURE_2D, 0);
}

template<class T, FREE_IMAGE_TYPE F>
Texture<T, F>::Texture(const std::string &file_name, int slot) {
  FIBITMAP *dib = BitmapFromFile(file_name.c_str(), width_, height_);
  textureUnit_ = slot;
  
  if (dib) {
    if (true) // always make sure that the loaded bitmap will fit the allocated data size
    {
      FIBITMAP *const dib_new = Convert(dib);
      assert(dib_new);
      FreeImage_Unload(dib);
      dib = dib_new;
    }
    
    data_.resize(size_t(width_) * size_t(height_));
    
    const int scan_width = FreeImage_GetPitch(dib); // (bytes)
    const int bpp = FreeImage_GetBPP(dib); // (bites)
    
    assert(bpp == sizeof(T) * 8);
    
    FreeImage_ConvertToRawBits((BYTE *) (data_.data()), dib, scan_width, bpp,
                               FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
    
    FreeImage_Unload(dib);
    dib = nullptr;
    
    double range[] = {(std::numeric_limits<double>::max)(), std::numeric_limits<double>::lowest()};
    
    for (const auto &pixel : data_) {
      range[0] = (std::min)(range[0], double(pixel.min_value()));
      range[1] = (std::max)(range[1], double(pixel.max_value()));
    }
    
    spdlog::info("[TEXTURE] Texture '{}' ({} x {} px, {} bpp, <{}, {}>, {} MB) loaded.\n",
                 file_name.c_str(), width_, height_, bpp, range[0], range[1],
                 scan_width * height_ / (1024.0f * 1024.0f));
    createGLTexture(slot);
  } else {
    std::stringstream ss;
    ss << "Texture '" << file_name << "' not loaded.";
    
    //std::cout << ss.str() << std::endl;
    throw std::runtime_error(ss.str());
  }
  //glBindTexture(GL_TEXTURE_2D, 0);
}

template<class T, FREE_IMAGE_TYPE F>
T Texture<T, F>::pixel(const int x, const int y) const {
  assert(x >= 0 && x < width_ && y >= 0 && y < height_);
  
  return data_[size_t(x) + size_t(y) * size_t(width_)];
}

template<class T, FREE_IMAGE_TYPE F>
void Texture<T, F>::Save(const std::string &file_name) const {
  FIBITMAP *bitmap = FreeImage_AllocateT(F, width_, height_, sizeof(T) * 8); // FIT_BITMAP, FIT_BITMAP, FIT_RGBF, FIT_RGBAF
  BYTE *data = (BYTE *) (FreeImage_GetBits(bitmap));
  const int scan_width = FreeImage_GetPitch(bitmap);
  memcpy(data, data_.data(), scan_width * height_);
  FreeImage_FlipVertical(bitmap);
  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(file_name.c_str());
  if (FreeImage_Save(fif, bitmap, file_name.c_str())) {
    printf("Texture has been saved successfully in '%s'.\n", file_name.c_str());
  } else {
    printf("Texture failed to save in '%s'.\n", file_name.c_str());
  }
  FreeImage_Unload(bitmap);
  bitmap = nullptr;
  data = nullptr;
}

template<class T, FREE_IMAGE_TYPE F>
FIBITMAP *Texture<T, F>::Convert(FIBITMAP *dib) {
  throw "Convert method is defined only for particular Texture types";
  
  return nullptr;
}

template<class T, FREE_IMAGE_TYPE F>
void Texture<T, F>::createGLTexture(int slot) {
  throw "createGLTexture method is defined only for particular Texture types";
  
}

template<>
void Texture3u::createGLTexture(int slot) {
  glCall(glGenTextures(1, &textureID_)); //reference to texture
  textureUnit_ = GL_TEXTURE0 + int(slot);
  glCall(glActiveTexture(textureUnit_)); //active texture unit 0
  glCall(glBindTexture(GL_TEXTURE_2D, textureID_)); //object texture_id is 2D texture
  glCall(glTexImage2D(GL_TEXTURE_2D, // Type of texture 1D, 2D, 3D
                      0, // Pyramid level (for mip-mapping) - 0 is the top level
                      GL_RGB32F, // Internal colour format to convert to
                      width_, // Image width i.e. 640
                      height_, // Image height i.e. 480
                      0, // Border width in pixels (can either be 1 or 0)
                      GL_RGB, // Input format (GL_RGB, GL_RGBA, GL_BGR etc.)
                      GL_UNSIGNED_BYTE, // Image data type
                      data())); // The actual image data itself
  glCall(glGenerateMipmap(GL_TEXTURE_2D)); //Generate mipmaps now!!!
}

template<>
void Texture4u::createGLTexture(int slot) {
  std::stringstream ss;
  ss << __PRETTY_FUNCTION__ << " not implemented yet";
  throw std::runtime_error(ss.str());
  //TODO
}

template<>
void Texture3f::createGLTexture(int slot) {
  
  glCall(glGenTextures(1, &textureID_)); //reference to texture
  textureUnit_ = GL_TEXTURE0 + int(slot);
  glCall(glActiveTexture(textureUnit_)); //active texture unit 0
  glCall(glBindTexture(GL_TEXTURE_2D, textureID_)); //object texture_id is 2D texture
  glCall(glTexImage2D(GL_TEXTURE_2D, // Type of texture 1D, 2D, 3D
                      0, // Pyramid level (for mip-mapping) - 0 is the top level
                      GL_RGB32F, // Internal colour format to convert to
                      width_, // Image width i.e. 640
                      height_, // Image height i.e. 480
                      0, // Border width in pixels (can either be 1 or 0)
                      GL_RGB, // Input format (GL_RGB, GL_RGBA, GL_BGR etc.)
                      GL_FLOAT, // Image data type
                      data())); // The actual image data itself
  glCall(glGenerateMipmap(GL_TEXTURE_2D)); //Generate mipmaps now!!!
}

template<>
void Texture4f::createGLTexture(int slot) {
  std::stringstream ss;
  ss << __PRETTY_FUNCTION__ << " not implemented yet";
  throw std::runtime_error(ss.str());
  //TODO
}

////////////////////////////////////////////////////////////////////////////////

template<class T, FREE_IMAGE_TYPE F>
void Texture<T, F>::createGLTexture(int slot, T *dataPtr,
                                    int mipmapLevel,
                                    bool genTextureId,
                                    unsigned int width,
                                    unsigned int height) {
  throw "createGLTexture method is defined only for particular Texture types";
  
}

template<>
void Texture3u::createGLTexture(int slot, Color3u *dataPtr,
                                int mipmapLevel,
                                bool genTextureId,
                                unsigned int width,
                                unsigned int height) {
  if (!glIsTexture(genTextureId)) {
    glCall(glGenTextures(1, &textureID_)); //reference to texture
    textureUnit_ = GL_TEXTURE0 + int(slot);
    
    glCall(glActiveTexture(textureUnit_)); //active texture unit 0
    glCall(glBindTexture(GL_TEXTURE_2D, textureID_)); //object texture_id is 2D texture
    
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, max_level));
  }
  //glActiveTexture(textureUnit_); //active texture unit 0
  //glBindTexture(GL_TEXTURE_2D, textureID_); //object texture_id is 2D texture
  glCall(glTexImage2D(GL_TEXTURE_2D, // Type of texture 1D, 2D, 3D
                      mipmapLevel, // Pyramid level (for mip-mapping) - 0 is the top level
                      GL_RGB32F, // Internal colour format to convert to
                      width, // Image width i.e. 640
                      height, // Image height i.e. 480
                      0, // Border width in pixels (can either be 1 or 0)
                      GL_RGB, // Input format (GL_RGB, GL_RGBA, GL_BGR etc.)
                      GL_UNSIGNED_BYTE, // Image data type
                      dataPtr)); // The actual image data itself
  //glGenerateMipmap(GL_TEXTURE_2D); //Generate mipmaps now!!!
}

template<>
void Texture4u::createGLTexture(int slot, Color4u *dataPtr,
                                int mipmapLevel,
                                bool genTextureId,
                                unsigned int width,
                                unsigned int height) {
  std::stringstream ss;
  ss << __PRETTY_FUNCTION__ << " not implemented yet";
  throw std::runtime_error(ss.str());
  //TODO
}

template<>
void Texture3f::createGLTexture(int slot, Color3f *dataPtr,
                                int mipmapLevel,
                                bool genTextureId,
                                unsigned int width,
                                unsigned int height) {
  if (!glIsTexture(textureID_)) {
    glCall(glGenTextures(1, &textureID_)); //reference to texture
    textureUnit_ = GL_TEXTURE0 + int(slot);
    
    glCall(glActiveTexture(textureUnit_)); //active texture unit 0
    glCall(glBindTexture(GL_TEXTURE_2D, textureID_)); //object texture_id is 2D texture
    
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, max_level));
  }
//
//  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
//  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  
  glCall(glTexImage2D(GL_TEXTURE_2D, // Type of texture 1D, 2D, 3D
                      mipmapLevel, // Pyramid level (for mip-mapping) - 0 is the top level
                      GL_RGB32F, // Internal colour format to convert to
                      width, // Image width i.e. 640
                      height, // Image height i.e. 480
                      0, // Border width in pixels (can either be 1 or 0)
                      GL_RGB, // Input format (GL_RGB, GL_RGBA, GL_BGR etc.)
                      GL_FLOAT, // Image data type
                      dataPtr)); // The actual image data itself
  //glGenerateMipmap(GL_TEXTURE_2D); //Generate mipmaps now!!!
}

template<>
void Texture4f::createGLTexture(int slot, Color4f *dataPtr,
                                int mipmapLevel,
                                bool genTextureId,
                                unsigned int width,
                                unsigned int height) {
  std::stringstream ss;
  ss << __PRETTY_FUNCTION__ << " not implemented yet";
  throw std::runtime_error(ss.str());
  //TODO
}
////////////////////////////////////////////////////////////////////////////////

template<class T, FREE_IMAGE_TYPE F>
T *Texture<T, F>::data() {
  return data_.data();
}

template<class T, FREE_IMAGE_TYPE F>
int Texture<T, F>::height() const {
  return height_;
}

template<class T, FREE_IMAGE_TYPE F>
int Texture<T, F>::width() const {
  return width_;
}

template<class T, FREE_IMAGE_TYPE F>
T Texture<T, F>::texel(const float u, const float v) const {
  T value;

#ifdef FAST_INTERP
  value = pixel(int(u * width_), int(v * height_)); // nearest neighbour
#else
  throw std::runtime_error("Bilinear interpolation not implemented yet");
// TODO bilinear interpolation
#endif
  
  return value;
}

template<class T, FREE_IMAGE_TYPE F>
unsigned int Texture<T, F>::getTextureId() const {
  return textureID_;
}

template<class T, FREE_IMAGE_TYPE F>
unsigned int Texture<T, F>::getTextureUnit() const {
  return textureUnit_;
}

template<class T, FREE_IMAGE_TYPE F>
Texture<T, F>::Texture(std::vector<std::string> filenames, int slot) {
  int index = 0;
  max_level = filenames.size() - 1;
  for (const std::string &file_name : filenames) {
    int width;
    int height;
    
    FIBITMAP *dib = BitmapFromFile(file_name.c_str(), width, height);
    
    if (dib) {
      //if (true) // always make sure that the loaded bitmap will fit the allocated data size
      {
        FIBITMAP *const dib_new = Convert(dib);
        assert(dib_new);
        FreeImage_Unload(dib);
        dib = dib_new;
      }
      
      std::vector<T> *dataPtr = nullptr;
      
      if (index == 0) {
        dataPtr = &data_;
        width_ = width;
        height_ = height;
        //data_.resize(size_t(width_) * size_t(height_));
      } else {
        dataPtr = new std::vector<T>();
      }
      
      dataPtr->resize(size_t(width) * size_t(height));
      
      const int scan_width = FreeImage_GetPitch(dib); // (bytes)
      const int bpp = FreeImage_GetBPP(dib); // (bites)
      
      assert(bpp == sizeof(T) * 8);
      
      FreeImage_ConvertToRawBits((BYTE *) (dataPtr->data()), dib, scan_width, bpp,
                                 FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
      
      FreeImage_Unload(dib);
      dib = nullptr;
      
      spdlog::info("[TEXTURE] '{}' ({} x {} px, {} bpp, {} MB) loaded.\n",
                   file_name.c_str(), width, height, bpp,
                   scan_width * height / (1024.0f * 1024.0f));
      //createGLTexture(slot);
      createGLTexture(slot, dataPtr->data(), index, index == 0, width, height);
      
      if (index != 0) {
        delete dataPtr;
      }
      dataPtr = nullptr;
    } else {
      std::stringstream ss;
      ss << "Texture '" << file_name << "' not loaded.";
      
      throw std::runtime_error(ss.str());
    }
    index++;
  }
  //glBindTexture(GL_TEXTURE_2D, 0);
  
}

template<class T, FREE_IMAGE_TYPE F>
void Texture<T, F>::setTextureUnit(unsigned int textureUnit) {
  textureUnit_ = GL_TEXTURE0 + textureUnit;
}

template<class T, FREE_IMAGE_TYPE F>
unsigned int Texture<T, F>::getMaxLevel() const {
  return max_level;
}

template<>
FIBITMAP *Texture3u::Convert(FIBITMAP *dib) {
  return FreeImage_ConvertTo24Bits(dib);
}

template<>
FIBITMAP *Texture4u::Convert(FIBITMAP *dib) {
  return FreeImage_ConvertTo32Bits(dib);
}

template<>
FIBITMAP *Texture3f::Convert(FIBITMAP *dib) {
  return Custom_FreeImage_ConvertToRGBF(dib);
}

template<>
FIBITMAP *Texture4f::Convert(FIBITMAP *dib) {
  return Custom_FreeImage_ConvertToRGBAF(dib);
}

template
class Texture<Color3f, FIT_RGBF>;

template
class Texture<Color3u, FIT_BITMAP>;

template
class Texture<Color4f, FIT_RGBAF>;

template
class Texture<Color4u, FIT_BITMAP>;