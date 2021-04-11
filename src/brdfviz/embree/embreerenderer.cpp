/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include <common/utils/rng.h>
#include <common/utils/rgb.h>
#include <common/utils/math.h>
#include <common/utils/assimploading.h>
#include "embreerenderer.h"
#include "rtccommonshader.h"
#include "rtcamera.h"
#include "pathtracerhelper.h"


EmbreeRenderer::EmbreeRenderer(const int width, const int height, const float renderScale) : width_(width), height_(height), renderScale_(renderScale) {
  texData_.reserve(width_ * height_);
  
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const int offset = (y * width_ + x);
      texData_[offset] = glm::vec4(0, 0, 1, 1);
    }
  }
  
  glCall(glCreateTextures(GL_TEXTURE_2D, 1, &texID_));
  glCall(glBindTexture(GL_TEXTURE_2D, texID_));
  glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width_, height_, 0, GL_RGBA, GL_FLOAT, texData_.data()));
  glCall(glBindTexture(GL_TEXTURE_2D, 0));
  
  defaultMtl_ = std::make_shared<Material>();
  
  commonShader_ = std::make_unique<RTCCommonShader>();
  commonShader_->pathTracerHelper = std::make_unique<PathTracerHelper>(width, height);
  
  camera_ = std::make_shared<RTCamera>(width, height, deg2rad(45.0), glm::vec3(5, 5, 5), glm::vec3(0, 0, 0));
  commonShader_->camera_ = camera_;
  
  mathScene_ = std::make_shared<MathScene>();
  mathScene_->initDefaultScene(defaultMtl_);
  commonShader_->mathScene_ = mathScene_;
  
  rtcScene_ = nullptr;
  commonShader_->rtcScene_ = nullptr;
  rtcDevice_ = nullptr;
  
  commonShader_->useShader = RTCShadingType::PathTracing;
}


void EmbreeRenderer::ui() {
  {
    std::lock_guard<std::mutex> lock(texDataLock_);
    
    glCall(glBindTexture(GL_TEXTURE_2D, texID_));
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width_, height_, 0, GL_RGBA, GL_FLOAT, texData_.data()));
    glCall(glBindTexture(GL_TEXTURE_2D, 0));
  }
  
  glm::vec3 oldCamPos = commonShader_->camera_->getViewFrom();
  
  ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoResize);
  ImGui::Image((void *) (intptr_t) texID_, ImVec2(float(width_) * renderScale_, float(height_) * renderScale_));
  
  if (ImGui::IsItemClicked(0)) {
    mouseInput = true;
  }
  ImGuiIO &io = ImGui::GetIO();
  
  if (mouseInput) {
    if (!io.MouseDown[0]) {
      mouseInput = false;
    } else {
      // Draw debug line
      ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button),
                                              4.0f);
      
      yaw += glm::radians(-io.MouseDelta.x);
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
  
  
  if (ImGui::Button("Invalidate")) {
    invalidateRendering();
  }
  
  if (ImGui::Button("Reset scene")) {
    std::lock_guard<std::mutex> lockScene(sceneLock_);
    releaseRTC();
    mathScene_->initDefaultScene(defaultMtl_);
    invalidateRendering();
  }
  
  if (ImGui::Button("Load scene")) {
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose Scene", ".obj", ".");
  }
  
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::lock_guard<std::mutex> lockScene(sceneLock_);
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
//      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      
      mathScene_->clearScene();
      initRTC();
      loadScene(filePathName);
      
      invalidateRendering();
    }
    
    // close
    ImGuiFileDialog::Instance()->Close();
  }
  
  if (ImGui::Button("Load test embree scene")) {
    std::lock_guard<std::mutex> lockScene(sceneLock_);
    mathScene_->clearScene();
    initRTC();
    loadTestScene();
    invalidateRendering();
  }
  
  ImGui::Text("Samples: %d", commonShader_->pathTracerHelper->getTracesCount());
  
  ImGui::Separator();
  glm::vec3 camTarget = commonShader_->camera_->view_at_;
  ImGui::Text("Camera info: ");
  ImGui::Text("  Zoom: %f", dist);
  ImGui::Text("  Pos: [%f,%f,%f]", oldCamPos[0], oldCamPos[1], oldCamPos[2]);
  ImGui::Text("  target: [%f,%f,%f]", camTarget[0], camTarget[1], camTarget[2]);
  
  ImGui::End();
  
  
  glm::vec3 newCamPos;
  
  newCamPos.x = std::cos(yaw) * std::cos(pitch);
  newCamPos.y = std::sin(yaw) * std::cos(pitch);
  newCamPos.z = std::sin(pitch);
  
  glm::normalize(newCamPos);
  newCamPos *= dist;
  commonShader_->camera_->view_from_ = newCamPos;
  if (oldCamPos != newCamPos) {
    invalidateRendering();
  }
  
}

glm::vec4 EmbreeRenderer::getPixel(int x, int y) {
  try {
    return commonShader_->getPixel(x, y);
  }
  catch (const std::exception &e) {
    spdlog::error("[EMBREE RENDERER] {}", e.what());
    return glm::vec4{rng(), rng(), rng(), 1.0f};
  }
}

void EmbreeRenderer::producer() {
  std::vector<glm::vec4> localData;
  localData.reserve(width_ * height_);
  
  float t = 0.0f; // time
  auto t0 = std::chrono::high_resolution_clock::now();
  
  while (!finishRequest_.load(std::memory_order_acquire)) {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> dt = t1 - t0;
    producerTime_ = dt.count();
    t += producerTime_;
    t0 = t1;
    
    // compute rendering

#pragma omp parallel for schedule(dynamic, 4) shared(localData)
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        std::lock_guard<std::mutex> lockScene(sceneLock_);
        const glm::vec4 pixel = getPixel(x, y);
        const int offset = (y * width_ + x);
        
        localData[offset].x = c_srgb(pixel.r, gamma_);
        localData[offset].y = c_srgb(pixel.g, gamma_);
        localData[offset].z = c_srgb(pixel.b, gamma_);
        localData[offset].w = 1;//c_srgb(pixel.a);
        
        if (finishRequest_.load(std::memory_order_acquire)) {
          return;
        }
        
        {
          std::lock_guard<std::mutex> lock(invalidateLock_);
          if (invalidate) {
            commonShader_->pathTracerHelper->resetTraces();
            x = 0;
            y = 0;
            invalidate = false;
          }
        }
      }
    }
    
    commonShader_->pathTracerHelper->incrementTraces();
    
    // write rendering results
    {
      std::lock_guard<std::mutex> lock(texDataLock_);
      memcpy(texData_.data(), localData.data(), width_ * height_ * sizeof(glm::vec4));
    } // lock release
  }
}

const std::atomic<bool> &EmbreeRenderer::getFinishRequest() const {
  return finishRequest_;
}

void EmbreeRenderer::finishRendering() {
  finishRequest_.store(true, std::memory_order_release);
}

std::unique_ptr<RTCCommonShader> &EmbreeRenderer::getCommonShader() {
  return commonShader_;
}

void EmbreeRenderer::invalidateRendering() {
  std::lock_guard<std::mutex> lock(invalidateLock_);
  invalidate = true;
}

EmbreeRenderer::~EmbreeRenderer() {
  releaseRTC();
}

void EmbreeRenderer::initRTC(const char *config) {
  rtcDevice_ = rtcNewDevice(config);
  rtcErrHandler(nullptr, rtcGetDeviceError(rtcDevice_), "Unable to create a new device.\n");
  rtcSetDeviceErrorFunction(rtcDevice_, rtcErrHandler, nullptr);
  
  ssize_t triangle_supported = rtcGetDeviceProperty(rtcDevice_, RTC_DEVICE_PROPERTY_TRIANGLE_GEOMETRY_SUPPORTED);
  
  // create a new scene bound to the specified device
  rtcScene_ = rtcNewScene(rtcDevice_);
  
  //robust ray tracing
  rtcSetSceneFlags(rtcScene_, RTC_SCENE_FLAG_ROBUST);
  
  rtcSetSceneBuildQuality(rtcScene_, RTC_BUILD_QUALITY_HIGH);
}

void EmbreeRenderer::loadTestScene() {
  
  // geometries are objects that represent an array of primitives of the same type, so lets create a triangle
  RTCGeometry mesh = rtcNewGeometry(rtcDevice_, RTC_GEOMETRY_TYPE_TRIANGLE);
  
  rtcSetGeometryUserData(mesh, &defaultMtl_);
  
  // and depending on the geometry type, different buffers must be bound (typically, vertex and index buffer is required)
  
  // set vertices in the newly created buffer
  glm::vec3 *vertices = (glm::vec3 *) rtcSetNewGeometryBuffer(
      mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(glm::vec3), 3);
  vertices[0].x = 0;
  vertices[0].y = 0;
  vertices[0].z = 0;
  
  vertices[1].x = 2;
  vertices[1].y = 0;
  vertices[1].z = 0;
  
  vertices[2].x = 0;
  vertices[2].y = 3;
  vertices[2].z = 0;
  
  // set triangle indices
  glm::vec<3, unsigned int> *triangles = (glm::vec<3, unsigned int> *) rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
                                                                                               sizeof(glm::vec<3, unsigned int>), 1);
  triangles[0].x = 0;
  triangles[0].y = 1;
  triangles[0].z = 2;
  
  // see also rtcSetSharedGeometryBuffer, rtcSetGeometryBuffer
  
  /*
  The parametrization of a triangle uses the first vertex p0 as base point, the vector (p1 - p0) as u-direction and the vector (p2 - p0) as v-direction.
  Thus vertex attributes t0, t1, t2 can be linearly interpolated over the triangle the following way:

  t_uv = (1-u-v)*t0 + u*t1 + v*t2	= t0 + u*(t1-t0) + v*(t2-t0)
  */
  
  // sets the number of slots (vertexAttributeCount parameter) for vertex attribute buffers (RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE)
  rtcSetGeometryVertexAttributeCount(mesh, 2);
  
  // set vertex normals
  glm::vec3 *normals = (glm::vec3 *) rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0,
                                                             RTC_FORMAT_FLOAT3,
                                                             sizeof(glm::vec3), 3);
  normals[0].x = 0;
  normals[0].y = 0;
  normals[0].z = 1;
  
  normals[1].x = 0;
  normals[1].y = 0;
  normals[1].z = 1;
  
  normals[2].x = 0;
  normals[2].y = 0;
  normals[2].z = 1;
  
  // set texture coordinates
  glm::vec2 *tex_coords = (glm::vec2 *) rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 1,
                                                                RTC_FORMAT_FLOAT2, sizeof(glm::vec2), 3);
  tex_coords[0].x = 0;
  tex_coords[0].y = 1;
  
  tex_coords[1].x = 1;
  tex_coords[1].y = 1;
  
  tex_coords[2].x = 0;
  tex_coords[2].y = 0;
  
  // changes to the geometry must be always committed
  rtcCommitGeometry(mesh);
  
  // geometries can be attached to a single scene
  unsigned int geom_id = rtcAttachGeometry(rtcScene_, mesh);
  // release geometry handle
  rtcReleaseGeometry(mesh);
  
  // commit changes to scene
  rtcCommitScene(rtcScene_);
  commonShader_->rtcScene_ = rtcScene_;
}

void EmbreeRenderer::loadScene(const std::string filename) {
  ObjLoaderSettings loaderSettings = {filename};
//  std::vector<AssimpLoadedModel> models;
  loadModel(loaderSettings, models);

//  using vec3ui = glm::vec<3, unsigned int>;
  
  for (const auto &model : models) {
    RTCGeometry mesh = rtcNewGeometry(rtcDevice_, RTC_GEOMETRY_TYPE_TRIANGLE);
    
    // and depending on the geometry type, different buffers must be bound (typically, vertex and index buffer is required)
    
    // set vertices in the newly created buffer
    glm::vec3 *vertices = (glm::vec3 *) rtcSetNewGeometryBuffer(
        mesh,
        RTC_BUFFER_TYPE_VERTEX,
        0,
        RTC_FORMAT_FLOAT3,
        sizeof(glm::vec3),
        model.vertices.size());
    
    // set triangle indices
    glm::vec<3, unsigned int> *triangles = (glm::vec<3, unsigned int> *) rtcSetNewGeometryBuffer(
        mesh,
        RTC_BUFFER_TYPE_INDEX,
        0,
        RTC_FORMAT_UINT3,
        sizeof(glm::vec<3, unsigned int>),
        model.indices.size());
    
    // see also rtcSetSharedGeometryBuffer, rtcSetGeometryBuffer
    rtcSetGeometryVertexAttributeCount(mesh, 2);
    
    glm::vec3 *normals = (glm::vec3 *) rtcSetNewGeometryBuffer(
        mesh,
        RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE,
        NORMAL_SLOT,
        RTC_FORMAT_FLOAT3,
        sizeof(glm::vec3),
        model.vertices.size());
    
    glm::vec2 *texcoords = (glm::vec2 *) rtcSetNewGeometryBuffer(
        mesh,
        RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE,
        TEXCOORD_SLOT,
        RTC_FORMAT_FLOAT2,
        sizeof(glm::vec2),
        model.vertices.size());

//    rtcSetGeometryUserData(mesh, (void *) (&model.material));
    rtcSetGeometryUserData(mesh, (void *) (&defaultMtl_));
    
    for (size_t i = 0; i < model.vertices.size(); i++) {
      const auto &vertex = model.vertices.at(i);
      
      vertices[i].x = vertex.Position.x;
      vertices[i].y = vertex.Position.y;
      vertices[i].z = vertex.Position.z;
      
      normals[i].x = vertex.Normal.x;
      normals[i].y = vertex.Normal.y;
      normals[i].z = vertex.Normal.z;
      
      texcoords[i].x = vertex.Texture.x;
      texcoords[i].y = vertex.Texture.y;
    }
    
    for (size_t i = 0; i < model.indices.size(); i++) {
      triangles[i].x = model.indices.at(i).x;
      triangles[i].y = model.indices.at(i).z;
      triangles[i].z = model.indices.at(i).y;
    }
    
    rtcCommitGeometry(mesh);
    unsigned int geomId = rtcAttachGeometry(rtcScene_, mesh);
    spdlog::info("[EMBREE RENDERER] Adding geometry {} to embree", geomId);
    rtcReleaseGeometry(mesh);
  }
  
  rtcCommitScene(rtcScene_);
  commonShader_->rtcScene_ = rtcScene_;
}

void EmbreeRenderer::releaseRTC() {
  commonShader_->rtcScene_ = nullptr;
  if (rtcScene_) {
    rtcReleaseScene(rtcScene_);
    rtcScene_ = nullptr;
  }
  
  if (rtcDevice_) {
    rtcReleaseDevice(rtcDevice_);
    rtcDevice_ = nullptr;
  }
  
}

void EmbreeRenderer::rtcErrHandler(void *user_ptr, const RTCError code, const char *str) {
  if (code != RTC_ERROR_NONE) {
    std::string descr = str ? ": " + std::string(str) : "";
    
    switch (code) {
      case RTC_ERROR_UNKNOWN: throw std::runtime_error("RTC_ERROR_UNKNOWN" + descr);
      case RTC_ERROR_INVALID_ARGUMENT: throw std::runtime_error("RTC_ERROR_INVALID_ARGUMENT" + descr);
      case RTC_ERROR_INVALID_OPERATION: throw std::runtime_error("RTC_ERROR_INVALID_OPERATION" + descr);
      case RTC_ERROR_OUT_OF_MEMORY: throw std::runtime_error("RTC_ERROR_OUT_OF_MEMORY" + descr);
      case RTC_ERROR_UNSUPPORTED_CPU: throw std::runtime_error("RTC_ERROR_UNSUPPORTED_CPU" + descr);
      case RTC_ERROR_CANCELLED: throw std::runtime_error("RTC_ERROR_CANCELLED" + descr);
      default: throw std::runtime_error("invalid error code" + descr);
    }
  }
}

std::shared_ptr<Material> &EmbreeRenderer::getDefaultMaterial() {
  return defaultMtl_;
}
