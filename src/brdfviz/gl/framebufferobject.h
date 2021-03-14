/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H


class FrameBufferObject {
public:
  FrameBufferObject(uint16_t width, uint16_t height);
  
  virtual ~FrameBufferObject();
  
  void generate();
  
  void bind() const;
  
  void unbind() const;
  
  bool resize(uint16_t width, uint16_t height);
  
  void saveScreen() const;
  
  uint32_t getWidth() const;
  
  uint32_t getHeight() const;
  
  uint32_t getFrameBufferId() const;
  
  uint32_t getFrameBufferColorId() const;
  
  uint32_t getFrameBufferDepthId() const;
  
  uint32_t getRealWidth() const;
  
  uint32_t getRealHeight() const;
  
  glm::vec2 getUV() const;
  
  float getRatio() const;

private:
  uint32_t width_;
  uint32_t height_;
  const uint32_t realWidth_ = 800;
  const uint32_t realHeight_ = 800;
  uint32_t frameBufferID = 0;
  uint32_t frameBufferColorID = 0;
  uint32_t frameBufferDepthID = 0;
  int32_t maxSize_[2];
};


#endif //FRAMEBUFFEROBJECT_H
