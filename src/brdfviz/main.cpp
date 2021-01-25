/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iostream>

#include "gui.h"

int main(int argc, const char **argv) {
  std::cout << "hello world\n";
  
  Gui gui("Example window");
  gui.render();
  
  return 0;
}