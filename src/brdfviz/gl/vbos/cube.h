/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/

#ifndef CUBE_H
#define CUBE_H

namespace vbo {
  namespace cube {
    const Vertex vertices[] = {
        Vertex{
            1.f, -1.f, -1.f,
            0.f, 1.f, 0.f
        },
        Vertex{
            -1.f, -1.f, -1.f,
            0.f, 1.f, 0.f
        },
        Vertex{
            -1.f, -1.f, 1.f,
            0.f, 1.f, 0.f
        },
        Vertex{
            1.f, -1.f, 1.f,
            0.f, 1.f, 0.f
        },
        Vertex{
            1.f, 1.f, -1.f,
            0.f, -1.f, 0.f
        },
        Vertex{
            1.f, 1.f, 1.f,
            0.f, -1.f, 0.f
        },
        Vertex{
            -1.f, 1.f, 1.f,
            0.f, -1.f, 0.f
        },
        Vertex{
            -1.f, 1.f, -1.f,
            0.f, -1.f, 0.f
        },
        Vertex{
            1.f, -1.f, -1.f,
            -1.f, -0.f, -0.f
        },
        Vertex{
            1.f, -1.f, 1.f,
            -1.f, -0.f, -0.f
        },
        Vertex{
            1.f, 1.f, 1.f,
            -1.f, -0.f, -0.f
        },
        Vertex{
            1.f, 1.f, -1.f,
            -1.f, -0.f, -0.f
        },
        Vertex{
            1.f, -1.f, 1.f,
            0.f, 0.f, -1.f
        },
        Vertex{
            -1.f, -1.f, 1.f,
            0.f, 0.f, -1.f
        },
        Vertex{
            -1.f, 1.f, 1.f,
            0.f, 0.f, -1.f
        },
        Vertex{
            1.f, 1.f, 1.f,
            0.f, 0.f, -1.f
        },
        Vertex{
            -1.f, -1.f, 1.f,
            1.f, 0.f, 0.f
        },
        Vertex{
            -1.f, -1.f, -1.f,
            1.f, 0.f, 0.f
        },
        Vertex{
            -1.f, 1.f, -1.f,
            1.f, 0.f, 0.f
        },
        Vertex{
            -1.f, 1.f, 1.f,
            1.f, 0.f, 0.f
        },
        Vertex{
            1.f, 1.f, -1.f,
            -0.f, -0.f, 1.f
        },
        Vertex{
            -1.f, 1.f, -1.f,
            -0.f, -0.f, 1.f
        },
        Vertex{
            -1.f, -1.f, -1.f,
            -0.f, -0.f, 1.f
        },
        Vertex{
            1.f, -1.f, -1.f,
            -0.f, -0.f, 1.f
        }
    };
    const unsigned int indices[] = {
        3, 0, 1, 1, 2, 3, 7, 4, 5, 5, 6, 7, 11, 8, 9, 9, 10, 11, 15, 12, 13, 13, 14, 15, 19, 16, 17, 17, 18, 19, 23, 20, 21,
        21, 22, 23
    };
  }
}


#endif //CUBE_H
