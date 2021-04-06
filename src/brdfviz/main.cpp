/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "gui.h"

#include "xmmintrin.h"
#include "pmmintrin.h"

int main() {
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
  
  std::unique_ptr<Gui> gui = std::make_unique<Gui>("Example window");
  gui->render();
  gui = nullptr;
  
  // GLFW has to be terminated AFTER deleting GUI
  glfwTerminate();
  return 0;
}