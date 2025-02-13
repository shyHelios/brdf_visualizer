/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/

#include <pch.h>
#include "shaderloader.h"

std::string ShaderLoader::loadFile(const char *fname) {
  std::ifstream file(fname);
  if (!file.is_open()) {
    spdlog::error("[SHADER] Unable to open file {}", fname);
    throw std::runtime_error("[SHADER] Unable to open shader");
  }
  std::stringstream fileData;
  fileData << file.rdbuf();
  file.close();
  return fileData.str();
}

GLint ShaderLoader::loadShader(const char *vertexFile, const char *fragmentFile) {
  spdlog::info("[SHADER] Compiling vs: {} fs: {}", vertexFile, fragmentFile);
  std::string vertexShaderString = loadFile(vertexFile);
  std::string fragmentShaderString = loadFile(fragmentFile);
  GLint vlen = vertexShaderString.length();
  GLint flen = fragmentShaderString.length();
  
  if (vertexShaderString.empty()) {
    spdlog::error("[SHADER] Null Vertex Shader");
    throw std::runtime_error("[SHADER] Null Vertex Shader");
  }
  if (fragmentShaderString.empty()) {
    spdlog::error("[SHADER] Null Fragment Shader");
    throw std::runtime_error("[SHADER] Null Fragment Shader");
  }
  
  
  const char *vertexShaderCStr = vertexShaderString.c_str();
  const char *fragmentShaderCStr = fragmentShaderString.c_str();
  
  glCall(vertexID = glCreateShader(GL_VERTEX_SHADER)); //Vertex Shader
  glCall(glShaderSource(vertexID, 1, (const GLchar **) &vertexShaderCStr, &vlen));
  glCall(glCompileShader(vertexID));
  GLint status;
  glCall(glGetShaderiv(vertexID, GL_COMPILE_STATUS, &status));
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glCall(glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &infoLogLength));
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glCall(glGetShaderInfoLog(vertexID, infoLogLength, nullptr, strInfoLog));
    spdlog::error("[SHADER] Compile failure in Vertex shader:\n{}", strInfoLog);
    delete[] strInfoLog;
    throw std::runtime_error("[SHADER] Compile failure in Vertex shader");
  }
  
  glCall(fragmentID = glCreateShader(GL_FRAGMENT_SHADER)); //Fragment Shader
  glCall(glShaderSource(fragmentID, 1, (const GLchar **) &fragmentShaderCStr, &flen));
  glCall(glCompileShader(fragmentID));
  
  glCall(glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &status));
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glCall(glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &infoLogLength));
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glCall(glGetShaderInfoLog(fragmentID, infoLogLength, nullptr, strInfoLog));
    spdlog::error("[SHADER] Compile failure in Fragment shader:\n{}", strInfoLog);
    delete[] strInfoLog;
    throw std::runtime_error("[SHADER] Compile failure in Fragment shader");
  }
  
  glCall(programID = glCreateProgram());
  glCall(glAttachShader(programID, vertexID));
  glCall(glAttachShader(programID, fragmentID));
  glCall(glLinkProgram(programID));
  
  glCall(glGetProgramiv(programID, GL_LINK_STATUS, &status));
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glCall(glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength));
    
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glCall(glGetProgramInfoLog(programID, infoLogLength, nullptr, strInfoLog));
    spdlog::error("[SHADER] Linker failure: {}", strInfoLog);
    delete[] strInfoLog;
    throw std::runtime_error("[SHADER] Linker failure");
  }
  
  return programID;
  
}

void ShaderLoader::deleteShader() {
  glCall(glDetachShader(programID, vertexID));
  glCall(glDetachShader(programID, fragmentID));
  glCall(glDeleteShader(vertexID));
  glCall(glDeleteShader(fragmentID));
  glCall(glUseProgram(0));
  glCall(glDeleteProgram(this->programID));
}
