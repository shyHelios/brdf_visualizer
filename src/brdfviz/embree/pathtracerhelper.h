/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef PATHTRACERHELPER_H
#define PATHTRACERHELPER_H

class PathTracerHelper {
public:
  PathTracerHelper(int height, int width);
  
  glm::vec4 getPixel(int row, int col);
  
  glm::vec4 getInterpolatedPixel(int row, int col);
  
  void incrementTraces();
  
  void resetTraces();
  
  void setPixel(int row, int col, const glm::vec4 &pix);
  
  int getTracesCount() const;

private:
  
  int width_;
  int height_;
  
  std::vector<glm::vec4> pixelsSum_;
  int traces_;
};


#endif //PATHTRACERHELPER_H
