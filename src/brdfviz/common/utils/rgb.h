/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RGB_H
#define RGB_H


template<class T_TYPE>
T_TYPE c_linear(T_TYPE c_srgb, float gamma = 2.4f);

template<class T_TYPE>
T_TYPE c_srgb(T_TYPE c_linear, float gamma = 2.4f);

template<>
float c_linear<float>(float c_srgb, float gamma);

template<>
float c_srgb<float>(float c_linear, float gamma);

template<>
glm::vec3 c_linear<glm::vec3>(glm::vec3 c_srgb, float gamma);

template<>
glm::vec3 c_srgb<glm::vec3>(glm::vec3 c_linear, float gamma);

template<>
glm::vec4 c_linear<glm::vec4>(glm::vec4 c_srgb, float gamma);

template<>
glm::vec4 c_srgb<glm::vec4>(glm::vec4 c_linear, float gamma);


#endif //RGB_H
