/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef EMBREERENDERER_H
#define EMBREERENDERER_H

#include "rtccommonshader.h"

class EmbreeRenderer {
public:
  EmbreeRenderer(const int width = 400, const int height = 400);
  
  glm::vec4 getPixel(int x, int y, float t = 0.0f);
  
  void ui();
  
  //virtual void saveImage(const std::string &filename);
  
  const std::atomic<bool> &getFinishRequest() const;
  
  void finishRendering();
  
  virtual void producer();

private:
  std::unique_ptr<RTCCommonShader> commonShader_;
  unsigned int texID_ = 0;
  float producerTime_;
  int width_{400};
  int height_{400};
  std::vector<glm::vec4> texData_;
  std::mutex texDataLock_;
  float gamma_;
  std::atomic<bool> finishRequest_{false};
};


#endif //EMBREERENDERER_H
