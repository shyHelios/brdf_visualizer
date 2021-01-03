/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/

#ifndef GIZMO_H
#define GIZMO_H

namespace vbo {
  namespace gizmo {
    const Vertex vertices[] = {
        Vertex{
            0.f, 0.f, 0.f,
            1.0f, 0.0f, 0.0f
        },
        Vertex{
            3.f, 0.f, 0.f,
            1.0f, 0.0f, 0.0f
        },
        
        Vertex{
            0.f, 0.f, 0.f,
            0.0f, 1.0f, 0.0f
        },
        Vertex{
            0.f, 3.f, 0.f,
            0.0f, 1.0f, 0.0f
        },
        
        Vertex{
            0.f, 0.f, 0.f,
            0.0f, 0.0f, 1.0f
        },
        Vertex{
            0.f, 0.f, 3.f,
            0.0f, 0.0f, 1.0f
        }
    };
    const unsigned int indices[] = {
        0, 1, 2, 3, 4, 5
    };
  }
};
#endif //GIZMO_H
