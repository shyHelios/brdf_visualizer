/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef GUI_H
#define GUI_H

#include "gl/openglrenderer.h"

typedef unsigned int GLuint;

struct GLFWwindow;

class FrameBufferObject;

class LineVertexBufferObject;

class BRDFShader;

/**
 * @brief Main GUI class for BRDF viz
 */
class Gui {
public:
  explicit Gui(const char *winName);
  
  virtual ~Gui();
  
  virtual void render();
  
  // static bool loadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);

protected:
  std::unique_ptr<FrameBufferObject> fbo_;
  std::unique_ptr<OpenGLRenderer> renderer;
  std::shared_ptr<LineVertexBufferObject> incidentVectorVBO;
  std::shared_ptr<LineVertexBufferObject> reflectedVectorVBO;
  std::shared_ptr<BRDFShader> brdfShader;
  
  virtual void ui();

//    virtual void initTextures() {}
  
  virtual void preRenderInit();
  
  virtual void renderLoop();

private:
  void init();
  
  const char *winName_;
  
  GLFWwindow *window;
  
  const char *glsl_version;
  
  static void glfw_error_callback(int error, const char *description);
};


#endif //GUI_H
