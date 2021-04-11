/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/

#ifndef PCH_H
#define PCH_H


#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory.h>
#include <ostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/quaternion.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuiFileDialog.h>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // aiScene output data structure
#include <assimp/postprocess.h> // Post processing flags

#include <embree3/rtcore.h>

#include <FreeImage.h>

// Glad - multi-Language GL/GLES/EGL/GLX/WGL loader-generator based on the official specs
#include <glad/glad.h>

// GLFW - simple API for creating windows, receiving input and events
#include <GLFW/glfw3.h>

#define FMT_HEADER_ONLY

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#if (NDEBUG)
  #if (NGL)
    #define glCall(x) (void)0
  #else
    #define glCall(x) x
  #endif
#else

#if defined(_MSC_VER)
  #define DBG_BREAK __debugbreak()
#else
  #define DBG_BREAK __builtin_trap()
#endif
  #if (NGL)
    #define glCall(x) (void)0
  #else
    #define glCall(x) \
    {               \
      glGetError(); \
      x;            \
      GLenum error = glGetError(); \
      if (error != GL_NO_ERROR){   \
        spdlog::error("[GL] error {} file '{}' line {}", error, __FILE__, __LINE__); \
        DBG_BREAK;\
      }\
    }
  #endif
#endif

#define M_2PI (M_PI + M_PI)
#define M_PI2 (M_PI * M_PI)

#define NORMAL_SLOT 0
#define TEXCOORD_SLOT 1

#define UP (glm::vec3(0.f,0.f,1.f))
#define TANG (glm::vec3(1.f,0.f,0.f))
#define BITANG (glm::vec3(0.f,1.f,0.f))

#define EPSILON static_cast<float>( 1e-5 )

#endif //PCH_H
