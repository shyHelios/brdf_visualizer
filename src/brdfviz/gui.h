/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef GUI_H
#define GUI_H


typedef unsigned int GLuint;

struct GLFWwindow;

class FrameBufferObject;

class LineVertexBufferObject;

class BRDFShader;

class EmbreeRenderer;

class OpenGLRenderer;

/**
 * @brief Main GUI class for BRDF viz
 */
class Gui {
public:
  explicit Gui(const char *winName);
  
  virtual ~Gui();
  
  virtual void render();

protected:
  std::unique_ptr<FrameBufferObject> fbo_;
  std::unique_ptr<OpenGLRenderer> renderer_;
  std::unique_ptr<EmbreeRenderer> embreeRenderer_;
  std::weak_ptr<LineVertexBufferObject> incidentVectorVBO_;
  std::weak_ptr<LineVertexBufferObject> reflectedVectorVBO_;
  std::weak_ptr<BRDFShader> brdfShader_;
  
  virtual void ui();
  
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
