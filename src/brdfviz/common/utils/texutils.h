/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef TEXUTILS_H
#define TEXUTILS_H

void saveScreen(const size_t width, const size_t height, const std::filesystem::path &path);

void saveScreen(const size_t width, const size_t height);

#endif //TEXUTILS_H
