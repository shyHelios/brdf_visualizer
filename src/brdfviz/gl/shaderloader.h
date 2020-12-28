/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef SHADERLOADER_H
#define SHADERLOADER_H


class ShaderLoader {
  unsigned int programID, vertexID, fragmentID;
  
  std::string loadFile(const char *fname);

public:
  int loadShader(const char *vertexFile, const char *fragmentFile);
  
  void deleteShader();
};

#endif //SHADERLOADER_H
