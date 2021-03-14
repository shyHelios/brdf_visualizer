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

class SamplerVisualizerObject;

class Object;

class Sampler;

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
  std::weak_ptr<SamplerVisualizerObject> samplerVisualizerObject_;
  std::weak_ptr<Object> brdfVizObject_;
  
  std::shared_ptr<Sampler> sampler_;
  
  bool showDemoWindow_ = false;
  ImVec2 mainRenderCanvasSize_;
  float yaw_ = M_PI / 4.0f;
  float pitch_ = M_PI / 4.0f;
  float dist_ = 7.0f;
  float theta_ = M_PI / 4.0f; // <0, PI/2>
  float phi_ = M_PI - (M_PI / 4.0f); // <0, 2*PI>
  bool mouseInput_ = false;
  bool geometry_ = false;
  bool shallInvalidateRTC_ = false;
  bool shallInvalidateSampler_ = false;
  bool shallSave_ = false;
  bool renderSampling_ = false;
  
  glm::vec3 normalVector = UP;
  glm::vec3 incidentVector = glm::vec3(0.5, 0.5, 0.5);
  glm::vec3 reflectedVector = glm::vec3(-0.5, -0.5, 0.5);
  
  
  virtual void ui();
  
  virtual void preRenderInit();
  
  virtual void renderLoop();

private:
  void init();
  
  void drawMainRender();
  
  void drawBRDFSettings();
  
  const char *winName_;
  
  GLFWwindow *window;
  
  const char *glsl_version;
  
  static void glfw_error_callback(int error, const char *description);
};


#endif //GUI_H
