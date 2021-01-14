/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include <gl/shaders/diffuseshader.h>
#include <gl/shaders/normalshader.h>
#include <gl/shaders/brdfshader.h>
#include "gl/camera.h"
#include "gl/object.h"
#include "gl/light.h"

#include "gl/vertexbufferobject.h"
#include "gl/linevertexbufferobject.h"
#include "gl/icospherevertexbufferobject.h"

#include "gl/framebufferobject.h"
#include "gl/openglscene.h"
#include "gui.h"

static bool show_demo_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static ImVec2 winSize;

static float yaw = M_PI / 4.0f;
static float pitch = M_PI / 4.0f;
//static float pitch = 0.3f;
static float dist = 7.0f;
static float thetha = M_PI / 4.0f; // <0, PI/2>
static float phi = M_PI - (M_PI / 4.0f); // <0, 2*PI>
static bool mouseInput = false;
static bool geometry = false;

static LineVertexBufferObject *incidentVectorVBO = nullptr;
static LineVertexBufferObject *reflectedVectorVBO = nullptr;
static BRDFShader *brdfShader = nullptr;

static bool shallSave = false;

void Gui::init() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    throw "Unable to init GLFW";
  
  // Decide GL+GLSL versions
#if __APPLE__
  // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
  // GL 3.0 + GLSL 130
  glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
  
  // Create window with graphics context
  window = glfwCreateWindow(1280, 720, winName_, NULL, NULL);
  if (window == NULL)
    throw "Unable to create window";
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync
  
  // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  bool err = gladLoadGL() == 0;
#else
  bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
  if (err) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    throw "Failed to initialize OpenGL loader";
  }
  
  VertexBufferObject::setupStaticObjects();
  LineVertexBufferObject::setupStaticObjects();
  
  fbo_ = new FrameBufferObject(800, 800);
  
  OpenGLScene *scene = new OpenGLScene();
  Camera *cam = new Camera();
  CameraTransformation *camTransformation = cam->transformation;
  camTransformation->overrideTarget.first = glm::vec3(10.f, 10.f, 0.f);
  camTransformation->overrideTarget.second = true;
  camTransformation->setPosition(glm::vec3(5, 5, 5));
  
  scene->addCamera(cam);
  
  Shader *defShader = new DiffuseShader("./Shaders/Default/VertexShader.glsl", "./Shaders/Default/FragmentShader.glsl");
  Shader *normalShader = new NormalShader("./Shaders/Normal/VertexShader.glsl", "./Shaders/Normal/FragmentShader.glsl");
  brdfShader = new BRDFShader("./Shaders/brdf/VertexShader.glsl", "./Shaders/brdf/FragmentShader.glsl");
  
  scene->addDefShader(defShader);
  
  defShader->addCamera(scene->getCamera());
  defShader->addLight(scene->getLights());
  
  normalShader->addCamera(scene->getCamera());
  normalShader->addLight(scene->getLights());
  
  brdfShader->addCamera(scene->getCamera());
  brdfShader->addLight(scene->getLights());
  
  
  scene->addObject(new Object(VertexBufferObject::cube, normalShader));
  scene->addObject(new Object(VertexBufferObject::cube, defShader));
  scene->addObject(new Object(VertexBufferObject::plane, defShader));
  
  incidentVectorVBO = new LineVertexBufferObject({glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5)}, {0, 1}, 4);
  reflectedVectorVBO = new LineVertexBufferObject({glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5)}, {0, 1}, 4);
  
  scene->addObject(
      new Object(incidentVectorVBO, defShader, nullptr,
                 new Material("Incident vector mtl",
                              Color3f{{0.1f, 0.1f, 0.1f}},
                              Color3f{{1.0f, 0.647f, 0.0f}})));
  
  scene->addObject(
      new Object(reflectedVectorVBO, defShader, nullptr,
                 new Material("Reflected vector mtl",
                              Color3f{{0.1f, 0.1f, 0.1f}},
                              Color3f{{0.0f, 0.349, 1.0f}})));
  
  scene->addObject(new Object(VertexBufferObject::disk, defShader));
  scene->addObject(new Object(new IcosphereVertexBufferObject(0), normalShader));
//  scene->addObject(new Object(new IcosphereVertexBufferObject(3), defShader));
//  scene->addObject(new Object(new IcosphereVertexBufferObject(6), brdfShader));
//  scene->addObject(new Object(new IcosphereVertexBufferObject(3), normalShader));
  scene->addObject(new Object(LineVertexBufferObject::gizmo, normalShader));
  
  scene->addLight(new Light(
      new Transformation(
          glm::vec3(0.f, 2000.f, 0.f)), //Pos
      glm::vec4(1.f), //Diffusion
      0.5f, //Ambient
      0.8f, //Specular
      LIGHT_INTENSITY_10)); //Intenzita
  
  renderer.addScene(scene, true);
}

void Gui::glfw_error_callback(int error, const char *description) {
  spdlog::error("[GLFW] {} {}", error, description);
}

void Gui::ui() {
  // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
  
  // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
  {
    ImGui::Begin("Hello, world!");// Create a window called "Hello, world!" and append into it.
    
    ImGui::Checkbox("Show Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    
    ImGui::ColorEdit3("clear color", (float *) &clear_color); // Edit 3 floats representing a color
    
    renderer.clearColor[0] = clear_color.x;
    renderer.clearColor[1] = clear_color.y;
    renderer.clearColor[2] = clear_color.z;
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    
    
    ImGui::Separator();
    ImGui::Text("Rendering");
    ImGui::Checkbox("Render edges", &geometry);
    shallSave = ImGui::Button("Save");
    ImGui::Separator();
    
    ImGui::Text("Camera info");
//    ImGui::SliderFloat("pitch", &pitch, -M_PI / 2., M_PI / 2.);
    ImGui::SliderFloat("yaw", &yaw, -2. * M_PI, 2. * M_PI);
    ImGui::SliderFloat("pitch", &pitch, -1.553, 1.553);
    ImGui::SliderFloat("dist", &dist, 0.0f, 100);
    
    ImGui::Separator();
    
    ImGui::Text("Incident beam");
    ImGui::SliderFloat("thetha", &thetha, 0, M_PI / 2.);
    ImGui::SliderFloat("phi", &phi, 0, 2. * M_PI);
    
    ImGui::Separator();
    ImGui::Text("BRDF");
    int *selectedIdx = reinterpret_cast<int *>(&brdfShader->currentBrdfIdx);
    
    ImGui::Combo("Selected BRDF",                           // const char* label,
                 selectedIdx,                              // int* current_item,
                 &BRDFShader::imguiSelectionGetter,         // bool(*items_getter)(void* data, int idx, const char** out_text),
                 (void *) BRDFShader::brdfArray,            // void* data
                 IM_ARRAYSIZE(BRDFShader::brdfArray));// int items_count
    
    switch (brdfShader->currentBrdfIdx) {
      case BRDFShader::BRDF::Phong:
      case BRDFShader::BRDF::BlinnPhong: {
        ImGui::SliderInt("Shininess", &brdfShader->getBrdfUniformLocations().shininess.getData(), 1, 100);
        break;
      }
      
      case BRDFShader::BRDF::TorranceSparrow: {
        ImGui::SliderFloat("Roughness", &brdfShader->getBrdfUniformLocations().roughness.getData(), 0.001, 0.2);
        ImGui::SliderFloat("f0", &brdfShader->getBrdfUniformLocations().f0.getData(), 0, 1);
        break;
      }
      
      case BRDFShader::BRDF::CountBrdf:break;
    }
    
    ImGui::End();
  }
  
  
  {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollWithMouse;
    ImGui::Begin("Test drawing into window", nullptr, window_flags);
    ImGuiIO &io = ImGui::GetIO();
    
    ImGui::Image((void *) fbo_->getFrameBufferId(), ImVec2(fbo_->getWidth(), fbo_->getHeight()));
    
    
    if (ImGui::IsItemClicked(0)) {
      mouseInput = true;
    }
    
    if (mouseInput) {
      if (!io.MouseDown[0]) {
        mouseInput = false;
      } else {
        // Draw debug line
        ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button),
                                                4.0f); // Draw a line between the button and the mouse cursor
        
        yaw += glm::radians(io.MouseDelta.x);
        pitch += glm::radians(io.MouseDelta.y);

//        pitch = std::min<float>(pitch, M_PI / 2.f);
//        pitch = std::max<float>(pitch, -M_PI / 2.f);
        
        pitch = std::min<float>(pitch, 1.553);
        pitch = std::max<float>(pitch, -1.553);
        
        if (yaw > 2 * M_PI) yaw -= 2 * M_PI;
        if (yaw < -2 * M_PI) yaw += 2 * M_PI;
        
      }
    }
    if (ImGui::IsItemHovered()) {
      dist -= (io.MouseWheel * 0.5f);
      dist = std::max(dist, 0.0f);
    }
    winSize = ImGui::GetWindowSize();
    ImGui::End();
  }
  
}

Gui::Gui(const char *winName) : winName_{winName} {
  init();
}

Gui::~Gui() {
  printf("Gui destructor\n");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  
  delete fbo_;
  
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Gui::render() {
  preRenderInit();
  renderLoop();
}

void Gui::preRenderInit() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
  //io.ConfigViewportsNoAutoMerge = true;
  //io.ConfigViewportsNoTaskBarIcon = true;
  
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();
  
  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
  
  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

//  initTextures();
}

void Gui::renderLoop() {
// Main loop
  
  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();
    
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    static bool p_open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    
    {
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
      
      ImGuiViewport *viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                      ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
      
      
      if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
      
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("DockSpace Demo", &p_open, window_flags);
      ImGui::PopStyleVar();
      
      ImGui::PopStyleVar(2);
      
      ImGuiIO &io = ImGui::GetIO();
      if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
      }
      
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
          
          if (ImGui::MenuItem("MenuItem1", "", false)) {
          
          }
          if (ImGui::MenuItem("MenuItem2", "", false)) {
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      ImGui::End();
    }
    //Ui routine to be overriden in child class
    
    
    //fbo_->resize(winSize.x, winSize.y);
    fbo_->bind();
    
    glm::vec3 newCamPos;
    
    newCamPos.x = std::cos(yaw) * std::cos(pitch);
    newCamPos.y = std::sin(yaw) * std::cos(pitch);
    newCamPos.z = std::sin(pitch);
    
    glm::normalize(newCamPos);
    newCamPos *= dist;
    
    renderer.getCurrentScene()->getCamera()->transformation->setPosition(newCamPos);
    {
      std::vector<Vertex> incidentVertices;
      std::vector<Vertex> reflectedVertices;
      std::vector<unsigned int> indices = {0, 1};
      
      glm::vec3 incidentVector = glm::vec3(
          std::sin(thetha) * std::cos(phi),
          std::sin(thetha) * std::sin(phi),
          std::cos(thetha));
      brdfShader->getBrdfUniformLocations().incidentVector.getData() = incidentVector;
      
      glm::vec3 reflectedVector = glm::vec3(-incidentVector[0], -incidentVector[1], incidentVector[2]);
      
      incidentVertices.emplace_back(glm::vec3(0, 0, 0));
      incidentVertices.emplace_back(incidentVector);
      
      reflectedVertices.emplace_back(glm::vec3(0, 0, 0));
      reflectedVertices.emplace_back(reflectedVector);
      
      
      incidentVectorVBO->recreate(incidentVertices, indices);
      reflectedVectorVBO->recreate(reflectedVertices, indices);
    }
    
    renderer.render(geometry);
    fbo_->unbind();
    if (shallSave) fbo_->saveScreen();
    
    ui();
    
    
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(window);
  }
}