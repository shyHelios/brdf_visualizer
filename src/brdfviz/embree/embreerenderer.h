/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef EMBREERENDERER_H
#define EMBREERENDERER_H

#include "rtccommonshader.h"

class EmbreeRenderer {
public:
  EmbreeRenderer(const int width = 400, const int height = 400, const float renderScale = 1.0f);
  
  virtual ~EmbreeRenderer() = default;
  
  glm::vec4 getPixel(int x, int y);
  
  void ui();
  
  //virtual void saveImage(const std::string &filename);
  
  const std::atomic<bool> &getFinishRequest() const;
  
  void finishRendering();
  
  virtual void producer();
  
  void invalidateRendering();
  
  std::unique_ptr<RTCCommonShader> &getCommonShader();

private:
  std::unique_ptr<RTCCommonShader> commonShader_;
  unsigned int texID_ = 0;
  float producerTime_;
  int width_;
  int height_;
  float renderScale_;
  std::vector<glm::vec4> texData_;
  std::mutex texDataLock_;
  float gamma_{1.4f};
  std::atomic<bool> finishRequest_{false};
  
  
  float yaw = M_PI / 4.0f;
  float pitch = M_PI / 4.0f;
  float dist = 3.0f;
  bool mouseInput = false;
  bool invalidate = false;
  std::mutex invalidateLock_;
};


#endif //EMBREERENDERER_H
