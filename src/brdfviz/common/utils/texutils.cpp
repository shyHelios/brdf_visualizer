/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "texutils.h"

void saveScreen(const size_t width, const size_t height) {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  
  std::stringstream ss;
  ss << "out/";
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
  ss << ".png";
  std::filesystem::path path(ss.str());
  
  saveScreen(width, height, path);
}

void saveScreen(const size_t width, const size_t height, const std::filesystem::path &path) {
  if (!std::filesystem::exists(path.parent_path())) {
    std::filesystem::create_directory(path.parent_path());
  }
  
  BYTE *pixels = (BYTE *) malloc(width * height * 3);
  
  glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);
  
  FIBITMAP *image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
  
  if (FreeImage_Save(FIF_PNG, image, (char*)path.c_str(), 0))
    spdlog::info("[TEXUTILS] Successfully saved!");
  else
    spdlog::error("[TEXUTILS] Failed saving!");
  
  FreeImage_Unload(image);
  
  free(pixels);
}
