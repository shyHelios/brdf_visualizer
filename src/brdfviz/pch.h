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
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//#include <assimp/Importer.hpp> // C++ importer interface
//#include <assimp/scene.h> // aiScene output data structure
//#include <assimp/postprocess.h> // Post processing flags

#include <FreeImage.h>

// Glad - multi-Language GL/GLES/EGL/GLX/WGL loader-generator based on the official specs
#include <glad/glad.h>

// GLFW - simple API for creating windows, receiving input and events
#include <GLFW/glfw3.h>

#define FMT_HEADER_ONLY

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#if (NDEBUG)
  #define glCall(x) x
#else

#if defined(_MSC_VER)
  #define DBG_BREAK __debugbreak()
#else
  #define DBG_BREAK __builtin_trap()
#endif

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

#endif //PCH_H
