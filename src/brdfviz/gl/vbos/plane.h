/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/

#ifndef PLANE_H
#define PLANE_H

namespace vbo {
  namespace plane {
    const Vertex vertices[] = {
        Vertex{
            1.f, -1.f, 0.f,
            0.f, 0.f, 1.f,
        },
        Vertex{
            -1.f, -1.f, 0.f,
            0.f, 0.f, 1.f,
        },
        Vertex{
            -1.f, 1.f, 0.f,
            0.f, 0.f, 1.f,
        },
        Vertex{
            1.f, 1.f, 0.f,
            0.f, 0.f, 1.f,
        }
    };
    const unsigned int indices[] = {
        3, 0, 1, 1, 2, 3
    };
  }
};
#endif //PLANE_H
