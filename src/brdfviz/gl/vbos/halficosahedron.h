/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/

#ifndef HALFICOSAHEDRON_H
#define HALFICOSAHEDRON_H


namespace vbo {
  namespace halficosahedron {
    const float X = .525731112119133606f;
    const float Z = .850650808352039932f;
    const float N = 0.f;
    
    const Vertex vertices[] = {
        Vertex{
            -X, Z, N,
            X, -Z, -N
        },
        Vertex{
            X, Z, N,
            -X, -Z, -N
        },
        Vertex{
            -X, -Z, N,
            X, Z, -N
        },
        Vertex{
            X, -Z, N,
            -X, Z, -N
        },
        Vertex{
            N, X, Z,
            -N, -X, -Z
        },
        Vertex{
            N, -X, Z,
            -N, X, -Z
        },
        Vertex{
            N, X, -Z,
            -N, -X, Z
        },
        Vertex{
            N, -X, -Z,
            -N, X, Z
        },
        Vertex{
            Z, N, X,
            -Z, -N, -X
        },
        Vertex{
            -Z, N, X,
            Z, -N, -X
        },
        Vertex{
            Z, N, N,
            -Z, -N, -N
        },
        Vertex{
            -Z, N, N,
            Z, -N, -N
        }
    };
    const unsigned int indices[] = {
        0, 1, 4,
        0, 4, 9,
        9, 4, 5,
        4, 8, 5,
        4, 1, 8,
        8, 1, 10,
        8, 10, 3,
        5, 8, 3,
        5, 3, 2,
//        2, 3, 7,
//        7, 3, 10,
//        7, 10, 6,
//        7, 6, 11,
//        11, 6, 0,
//        0, 6, 1,
//        6, 10, 1,
        9, 11, 0,
        9, 2, 11,
        9, 5, 2,
//        7, 11, 2,
    };
  }
}

#endif //HALFICOSAHEDRON_H
