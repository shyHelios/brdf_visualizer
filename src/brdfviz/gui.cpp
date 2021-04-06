/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "gui.h"

#include <gl/shaders/diffuseshader.h>
#include <gl/shaders/normalshader.h>
#include <gl/shaders/brdfshader.h>
#include <gl/samplervisualizerobject.h>
#include <common/utils/coordsystem.h>

#include "gl/camera.h"
#include "gl/object.h"
#include "gl/light.h"

#include "gl/vertexbufferobject.h"
#include "gl/linevertexbufferobject.h"
#include "gl/icospherevertexbufferobject.h"

#include "gl/framebufferobject.h"
#include "gl/openglscene.h"

#include "gl/vbos/cube.h"
#include "gl/vbos/plane.h"
#include "gl/vbos/icosahedron.h"
#include "gl/vbos/halficosahedron.h"
#include "gl/vbos/gizmo.h"

#include "gl/openglrenderer.h"
#include "embree/embreerenderer.h"


void Gui::init() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    throw "Unable to init GLFW";
  
  glsl_version = "#version 460";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  
  // Create window with graphics context
  window = glfwCreateWindow(1280, 720, winName_, NULL, NULL);
  if (window == NULL)
    throw "Unable to create window";
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync
  
  // Initialize OpenGL loader
  bool err = gladLoadGL() == 0;
  
  if (err) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    throw "Failed to initialize OpenGL loader";
  }

//  VertexBufferObject::setupStaticObjects();
  std::shared_ptr<VertexBufferObject> cube = nullptr;
  std::shared_ptr<VertexBufferObject> plane = nullptr;
  std::shared_ptr<VertexBufferObject> disk = nullptr;
  std::shared_ptr<VertexBufferObject> icosahedron = nullptr;
  std::shared_ptr<VertexBufferObject> halficosahedron = nullptr;
  std::shared_ptr<LineVertexBufferObject> gizmo = nullptr;
  
  cube = std::make_shared<VertexBufferObject>(
      std::vector<Vertex>(std::begin(vbo::cube::vertices), std::end(vbo::cube::vertices)),
      std::vector<unsigned int>(std::begin(vbo::cube::indices), std::end(vbo::cube::indices)));
  
  plane = std::make_shared<VertexBufferObject>(
      std::vector<Vertex>(std::begin(vbo::plane::vertices), std::end(vbo::plane::vertices)),
      std::vector<unsigned int>(std::begin(vbo::plane::indices), std::end(vbo::plane::indices)));
  
  icosahedron = std::make_shared<VertexBufferObject>(
      std::vector<Vertex>(std::begin(vbo::icosahedron::vertices), std::end(vbo::icosahedron::vertices)),
      std::vector<unsigned int>(std::begin(vbo::icosahedron::indices), std::end(vbo::icosahedron::indices)));
  
  halficosahedron = std::make_shared<VertexBufferObject>(
      std::vector<Vertex>(std::begin(vbo::halficosahedron::vertices), std::end(vbo::halficosahedron::vertices)),
      std::vector<unsigned int>(std::begin(vbo::halficosahedron::indices), std::end(vbo::halficosahedron::indices)));
  
  //generate disk
  {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    const unsigned int resolution = 80;
    double radiansAngle = 0;
    const double radiansFullAngle = 2. * M_PI;
    const double radius = 1.;
    const double radiansStep = radiansFullAngle / static_cast<double>(resolution);
    vertices.emplace_back(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // center
    for (unsigned int i = 0; i < resolution; i++) {
      vertices.emplace_back(glm::vec3(radius * std::cos(radiansAngle), radius * std::sin(radiansAngle), 0), glm::vec3(0, 1, 0));
//      vertices.emplace_back(glm::vec3(radius * std::cos(radiansAngle), 0, radius * std::sin(radiansAngle)), glm::vec3(std::cos(radiansAngle), 0, std::sin(radiansAngle)));
      radiansAngle += radiansStep;

//      int vidx1 = i;
//      int vidx2 = (i + 1) % resolution;
      indices.emplace_back(0);
      indices.emplace_back(((i + 1) % resolution) + 1);
      indices.emplace_back(i + 1);
    }
    
    disk = std::make_shared<VertexBufferObject>(vertices, indices);
  }
  gizmo = std::make_shared<LineVertexBufferObject>(
      std::vector<Vertex>(std::begin(vbo::gizmo::vertices), std::end(vbo::gizmo::vertices)),
      std::vector<unsigned int>(std::begin(vbo::gizmo::indices), std::end(vbo::gizmo::indices)),
      5);

//  LineVertexBufferObject::setupStaticObjects();
  renderer_ = std::make_unique<OpenGLRenderer>();
  embreeRenderer_ = std::make_unique<EmbreeRenderer>(200, 200, 2.0f);
//  embreeRenderer_ = std::make_unique<EmbreeRenderer>(400, 400, 1.0f);
  
  fbo_ = std::make_unique<FrameBufferObject>(800, 800);
  
  std::shared_ptr<OpenGLScene> scene = std::make_shared<OpenGLScene>();
  
  std::shared_ptr<Camera> cam = std::make_shared<Camera>();
  std::shared_ptr<CameraTransformation> camTransformation = cam->transformation;
  camTransformation->overrideTarget.first = glm::vec3(10.f, 10.f, 0.f);
  camTransformation->overrideTarget.second = true;
  camTransformation->setPosition(glm::vec3(5, 5, 5));
  
  scene->addCamera(cam);
  
  std::shared_ptr<Shader> defShader = std::make_shared<DiffuseShader>("./Shaders/Default/VertexShader.glsl", "./Shaders/Default/FragmentShader.glsl");
  std::shared_ptr<Shader> normalShader = std::make_shared<NormalShader>("./Shaders/Normal/VertexShader.glsl", "./Shaders/Normal/FragmentShader.glsl");
  auto brdfShader = std::make_shared<BRDFShader>("./Shaders/brdf/VertexShader.glsl", "./Shaders/brdf/FragmentShader.glsl");
  brdfShader_ = brdfShader; // assign to weak ptr
  embreeRenderer_->getCommonShader()->brdfShader = brdfShader_;
  scene->addDefShader(defShader);
  
  
  defShader->addCamera(cam);
  defShader->addLight(scene->getLights());
  cam->addShader(defShader);
  
  normalShader->addCamera(scene->getCamera());
  normalShader->addLight(scene->getLights());
  cam->addShader(normalShader);
  
  brdfShader->addCamera(scene->getCamera());
  brdfShader->addLight(scene->getLights());
  cam->addShader(brdfShader);


//  scene->addObject(std::make_shared<Object>(VertexBufferObject::cube, normalShader));
//  scene->addObject(std::make_shared<Object>(VertexBufferObject::cube, defShader));
//  scene->addObject(std::make_shared<Object>(VertexBufferObject::plane, defShader));
  
  const std::vector<Vertex> vertices = {glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5)};
  const std::vector<unsigned int> indices = {0, 1};
  
  auto incidentVectorVBO = std::make_shared<LineVertexBufferObject>(vertices, indices, 4);
  auto reflectedVectorVBO = std::make_shared<LineVertexBufferObject>(vertices, indices, 4);
  
  
  incidentVectorVBO_ = incidentVectorVBO; // Assign to weak ptr
  reflectedVectorVBO_ = reflectedVectorVBO; // Assign to weak ptr
  
  scene->addObject(
      std::make_shared<Object>(incidentVectorVBO, defShader, nullptr,
                               std::make_shared<Material>("Incident vector mtl",
                                                          Color3f{{0.1f, 0.1f, 0.1f}},
                                                          Color3f{{1.0f, 0.647f, 0.0f}})));
  
  scene->addObject(
      std::make_shared<Object>(reflectedVectorVBO, defShader, nullptr,
                               std::make_shared<Material>("Reflected vector mtl",
                                                          Color3f{{0.1f, 0.1f, 0.1f}},
                                                          Color3f{{0.0f, 0.349, 1.0f}})));
  
  
  sampler_ = std::make_shared<Sampler>(brdfShader);
  embreeRenderer_->getCommonShader()->setSampler(sampler_);
  
  auto samplerVisualizerObject = std::make_shared<SamplerVisualizerObject>(normalVector,
                                                                           incidentVector,
                                                                           reflectedVector,
                                                                           sampler_,
                                                                           nullptr,
                                                                           normalShader);
  samplerVisualizerObject->setVisible(false);
  samplerVisualizerObject_ = samplerVisualizerObject;
//  samplerVisualizerObject->normal
  scene->addObject(samplerVisualizerObject);
  scene->addObject(std::make_shared<Object>(disk, defShader));
//  scene->addObject(std::make_shared<Object>(std::make_shared<IcosphereVertexBufferObject>(0), normalShader));
//  scene->addObject(std::make_shared<Object>(std::make_shared<IcosphereVertexBufferObject>(3), defShader));
//  scene->addObject(std::make_shared<Object>(std::make_shared<IcosphereVertexBufferObject>(3), normalShader));
  
  auto brdfVizObject = std::make_shared<Object>(std::make_shared<IcosphereVertexBufferObject>(6), brdfShader);
  brdfVizObject_ = brdfVizObject;
  scene->addObject(brdfVizObject);
  scene->addObject(std::make_shared<Object>(gizmo, normalShader));
  
  scene->addLight(std::make_shared<Light>(
      std::make_shared<Transformation>(
          glm::vec3(0.f, 2000.f, 0.f)), //Pos
      glm::vec4(1.f), //Diffusion
      0.5f, //Ambient
      0.8f, //Specular
                      LIGHT_INTENSITY_10),
                  cube); //Intenzita
  
  renderer_->addScene(scene, true);
  
}

void Gui::glfw_error_callback(int error, const char *description) {
  spdlog::error("[GLFW] {} {}", error, description);
}

void Gui::ui() {
  // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
  if (showDemoWindow_)
    ImGui::ShowDemoWindow(&showDemoWindow_);
  
  // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
  {
    ImGui::Begin("Hello, world!");// Create a window called "Hello, world!" and append into it.
    
    ImGui::Checkbox("Show Demo Window", &showDemoWindow_);      // Edit bools storing our window open/close state
    
    ImGui::ColorEdit3("clear color", (float *) &renderer_->clearColor); // Edit 3 floats representing a color
    
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    
    
    ImGui::Separator();
    ImGui::Text("Rendering");
    ImGui::Checkbox("Render edges", &geometry_);
    shallSave_ = ImGui::Button("Save");
    
    ImGui::Separator();
    
    #pragma region "Camera settings"
    ImGui::Text("Camera info");
//    ImGui::SliderFloat("pitch", &pitch, -M_PI / 2., M_PI / 2.);
    ImGui::SliderFloat("yaw", &yaw_, -2. * M_PI, 2. * M_PI);
    ImGui::SliderFloat("pitch", &pitch_, -1.553, 1.553);
    ImGui::SliderFloat("dist", &dist_, 0.0f, 100);
    #pragma endregion
    
    ImGui::Separator();
    
    #pragma region "Light input"
    ImGui::Text("Incident beam");
    shallInvalidateSampler_ |= ImGui::SliderFloat("theta", &theta_, 0, M_PI / 2.);
    shallInvalidateSampler_ |= ImGui::SliderFloat("phi", &phi_, 0, 2. * M_PI);
    #pragma endregion
    
    ImGui::Separator();
    
    #pragma region "Furnace test"
    bool &furnaceTest = embreeRenderer_->getCommonShader()->getUseSphereMapRef();
    glm::vec4 &rtcFurnaceBackground = embreeRenderer_->getCommonShader()->getDefaultBgColorRef();
    shallInvalidateRTC_ |= ImGui::Checkbox("Enable IBL", &furnaceTest);
    if (!furnaceTest)
      shallInvalidateRTC_ |= ImGui::ColorEdit3("Furnace test bg", (float *) &rtcFurnaceBackground); // Edit 3 floats representing a color
    #pragma endregion
    
    ImGui::Separator();
    
    #pragma region "Render type"
    ImGui::Text("Rendering");
    ImGui::Checkbox("Render sampling", &renderSampling_);
    
    if (auto brdfVizObjPtr = brdfVizObject_.lock()) {
      brdfVizObjPtr->setVisible(!renderSampling_);
    }
    
    if (auto samplerVisualizerObjPtr = samplerVisualizerObject_.lock()) {
      samplerVisualizerObjPtr->setVisible(renderSampling_);
      if (renderSampling_) {
        
        shallInvalidateSampler_ |= ImGui::Checkbox("Multiply by pdf", &samplerVisualizerObjPtr->getMultiplyByPdf());
        
        shallInvalidateSampler_ |= ImGui::SliderInt("Resolution", &samplerVisualizerObjPtr->getResolution(), 0, 100);
      }
    }
    
    #pragma endregion
    
    ImGui::Separator();
    
    drawBRDFSettings();
    
    #pragma region "Sampler"
    
    ImGui::Separator();
    ImGui::Text("Sampling");
    
    bool samplerSelected = false;
    
    int *selectedIdx = reinterpret_cast<int *>(&sampler_->getCurrentType());
    
    samplerSelected |= ImGui::Combo("Selected sampler",                                 // const char* label,
                                    selectedIdx,                                              // int* current_item,
                                    &Sampler::imguiSelectionGetter,                           // bool(*items_getter)(void* data, int idx, const char** out_text),
                                    (void *) Sampler::samplerTypeArray,                       // void* data
                                    IM_ARRAYSIZE(Sampler::samplerTypeArray));// int items_count
    shallInvalidateSampler_ |= samplerSelected;
    shallInvalidateRTC_ |= samplerSelected;
    
    bool sampleLights = false;
    
    shallInvalidateRTC_ |= ImGui::Checkbox("Sample lights", &sampleLights);
    
    
    #pragma endregion
    
    ImGui::End();
  }
  
  drawMainRender();
  
}

Gui::Gui(const char *winName) : winName_{winName} {
  init();
}

Gui::~Gui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  
  // GLFW has to be terminated AFTER destructor
  //glfwTerminate();
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
  
  //clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
  
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
  std::thread producerThread(&EmbreeRenderer::producer, this->embreeRenderer_.get());
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
      ImGui::End();
    }
    
    auto cam = renderer_->getCurrentScene()->getCamera();
    cam->ratio = fbo_->getRatio();
    
    glm::vec3 newCamPos;
    
    newCamPos.x = std::cos(yaw_) * std::cos(pitch_);
    newCamPos.y = std::sin(yaw_) * std::cos(pitch_);
    newCamPos.z = std::sin(pitch_);
    
    glm::normalize(newCamPos);
    newCamPos *= dist_;
    
    cam->transformation->setPosition(newCamPos);
    {
      std::vector<Vertex> incidentVertices;
      std::vector<Vertex> reflectedVertices;
      std::vector<unsigned int> indices = {0, 1};
      
      incidentVector = sphericalToCartesian(theta_, phi_);
      
      if (auto brdfShader = brdfShader_.lock()) {
        brdfShader->getBrdfUniformLocations().incidentVector.getData() = incidentVector;
      } else {
        spdlog::warn("[GUI] BRDFShader ptr expired");
      }
      
      reflectedVector = glm::vec3(-incidentVector[0], -incidentVector[1], incidentVector[2]);
      
      incidentVertices.emplace_back(glm::vec3(0, 0, 0));
      incidentVertices.emplace_back(incidentVector);
      
      reflectedVertices.emplace_back(glm::vec3(0, 0, 0));
      reflectedVertices.emplace_back(reflectedVector);
      
      
      if (auto incidentVectorVBO = incidentVectorVBO_.lock()) {
        incidentVectorVBO->recreate(incidentVertices, indices);
      } else {
        spdlog::warn("[GUI] incidentVectorVBO ptr expired");
      }
      
      if (auto reflectedVectorVBO = reflectedVectorVBO_.lock()) {
        reflectedVectorVBO->recreate(reflectedVertices, indices);
      } else {
        spdlog::warn("[GUI] reflectedVectorVBO ptr expired");
      }
    }
    
    if (shallInvalidateRTC_) {
      embreeRenderer_->invalidateRendering();
      shallInvalidateRTC_ = false;
    }
    
    if (auto samplerVisualizerObjPtr = samplerVisualizerObject_.lock()) {
      if (renderSampling_ && shallInvalidateSampler_) {
        samplerVisualizerObjPtr->update();
        shallInvalidateSampler_ = false;
      }
    }
    
    // If saving screen, resize fbo to real size, render again and resize it back.
    if (shallSave_) {
      fbo_->resize(fbo_->getRealWidth(), fbo_->getRealHeight());
      fbo_->bind();
      renderer_->render(geometry_);
      fbo_->unbind();
      fbo_->saveScreen();
      shallSave_ = false;
    }
    
    fbo_->resize(mainRenderCanvasSize_.x, mainRenderCanvasSize_.y);
    fbo_->bind();
    renderer_->render(geometry_);
    fbo_->unbind();


//    shallInvalidateRTC_ = false;
//    shallInvalidateSampler_ = false;
    ui();
    embreeRenderer_->ui();
    
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
//    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClearColor(1, 1, 1, 1);
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
  embreeRenderer_->finishRendering();
  producerThread.join();
}

void Gui::drawMainRender() {
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollWithMouse;
  ImGui::Begin("Test drawing into window", nullptr, window_flags);
  ImGuiIO &io = ImGui::GetIO();
  
  glm::vec2 fboUv = fbo_->getUV();
  ImGui::Image((void *) (intptr_t) fbo_->getFrameBufferColorId(),
               ImVec2(fbo_->getWidth(), fbo_->getHeight()),
               ImVec2(0, 0),
               ImVec2(fboUv.x, fboUv.y));
  
  
  if (ImGui::IsItemClicked(0)) {
    mouseInput_ = true;
  }
  
  if (mouseInput_) {
    if (!io.MouseDown[0]) {
      mouseInput_ = false;
    } else {
      // Draw debug line
      ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button),
                                              4.0f);
      
      yaw_ += glm::radians(io.MouseDelta.x);
      pitch_ += glm::radians(io.MouseDelta.y);
      
      pitch_ = std::min<float>(pitch_, 1.553);
      pitch_ = std::max<float>(pitch_, -1.553);
      
      if (yaw_ > 2 * M_PI) yaw_ -= 2 * M_PI;
      if (yaw_ < -2 * M_PI) yaw_ += 2 * M_PI;
    }
  }
  
  if (ImGui::IsItemHovered()) {
    dist_ -= (io.MouseWheel * 0.5f);
    dist_ = std::max(dist_, 0.0f);
  }
  
  mainRenderCanvasSize_ = ImGui::GetWindowSize();
  ImGui::End();
}

void Gui::drawBRDFSettings() {
  if (auto brdfShader = brdfShader_.lock()) {
    using Phong = BRDFShader::PhongUniformLocationsPack;
    using TorranceSparrow = BRDFShader::TorranceSparrowUniformLocationsPack;
    using Lambert = BRDFShader::LambertUniformLocationsPack;
    using OrenNayar = BRDFShader::OrenNayarUniformLocationsPack;
    
    ImGui::Text("BRDF");
    int *selectedIdx = reinterpret_cast<int *>(&brdfShader->currentBrdfIdx);
    
    shallInvalidateRTC_ |= ImGui::Combo("Selected BRDF",                           // const char* label,
                                        selectedIdx,                              // int* current_item,
                                        &BRDFShader::imguiSelectionGetter,         // bool(*items_getter)(void* data, int idx, const char** out_text),
                                        (void *) BRDFShader::brdfArray,            // void* data
                                        IM_ARRAYSIZE(BRDFShader::brdfArray));// int items_count
    switch (brdfShader->currentBrdfIdx) {
      case BRDFShader::BRDF::Phong:
      case BRDFShader::BRDF::BlinnPhong:
      case BRDFShader::BRDF::PhongPhysCorrect: {
        bool shininessChanged = ImGui::SliderInt("Shininess", &brdfShader->getBrdfUniformLocations().Phong::shininess.getData(), 1, 100);
        shallInvalidateRTC_ |= shininessChanged;
        shallInvalidateSampler_ |= shininessChanged;
        if (brdfShader->currentBrdfIdx == BRDFShader::BRDF::PhongPhysCorrect) {
          const bool specularChanged = ImGui::SliderFloat("Specular", &brdfShader->getBrdfUniformLocations().Phong::specular.getData(), 0,
                                                          1);
          const bool diffuseChanged = ImGui::SliderFloat("Diffuse", &brdfShader->getBrdfUniformLocations().Phong::diffuse.getData(), 0,
                                                         1);
          
          if (specularChanged) {
            brdfShader->getBrdfUniformLocations().Phong::diffuse.getData() = std::min(brdfShader->getBrdfUniformLocations().Phong::diffuse.getData(),
                                                                                      1.f - brdfShader->getBrdfUniformLocations().Phong::specular.getData());
          } else if (diffuseChanged) {
            brdfShader->getBrdfUniformLocations().Phong::specular.getData() = std::min(brdfShader->getBrdfUniformLocations().Phong::specular.getData(),
                                                                                       1.f - brdfShader->getBrdfUniformLocations().Phong::diffuse.getData());
          }
          
          shallInvalidateRTC_ |= specularChanged;
          shallInvalidateRTC_ |= diffuseChanged;
          shallInvalidateSampler_ |= specularChanged;
          shallInvalidateSampler_ |= diffuseChanged;
        }
        break;
      }
      
      case BRDFShader::BRDF::TorranceSparrow: {
        shallInvalidateRTC_ |= ImGui::SliderFloat("Roughness", &brdfShader->getBrdfUniformLocations().TorranceSparrow::roughness.getData(), 0.001, 1.0);
        shallInvalidateRTC_ |= ImGui::SliderFloat("f0", &brdfShader->getBrdfUniformLocations().TorranceSparrow::f0.getData(), 0, 1);
        break;
      }
      
      case BRDFShader::BRDF::Lambert: {
        shallInvalidateRTC_ |= ImGui::SliderFloat("Reflectance", &brdfShader->getBrdfUniformLocations().Lambert::reflectance.getData(), 0., 1.0);
        break;
      }
      
      case BRDFShader::BRDF::OrenNayar: {
        shallInvalidateRTC_ |= ImGui::SliderFloat("Roughness", &brdfShader->getBrdfUniformLocations().OrenNayar::roughness.getData(), 0, 1.0);
        shallInvalidateRTC_ |= ImGui::SliderFloat("Reflectance", &brdfShader->getBrdfUniformLocations().OrenNayar::reflectance.getData(), 0., 1.0);
        break;
      }
      
      case BRDFShader::BRDF::Mirror: { break; }
      case BRDFShader::BRDF::CountBrdf: { break; }
    }
  } else {
    spdlog::warn("[GUI] BRDFShader ptr expired");
  }
}
