/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "gui.h"

int main() {
  Gui gui("Example window");
  gui.render();
  return 0;
}