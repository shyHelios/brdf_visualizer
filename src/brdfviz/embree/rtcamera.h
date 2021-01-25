/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RTCAMERA_H
#define RTCAMERA_H


class RTCamera {
public:
  RTCamera(int width, int height, float fov_y,
           const glm::vec3 &view_from, glm::vec3 view_at);
  
  /* generate primary ray, top-left pixel image coordinates (xi, yi) are in the range <0, 1) x <0, 1) */
  RTCRay GenerateRay(const float xi, const float yi);
  
  
  int getWidth() const;
  
  int getHeight() const;
  
  float getFovY() const;
  
  const glm::vec3 getRayDirection(float x_i, float y_i);
  
  const glm::vec3 &getViewFrom() const;
  
  const glm::vec3 &getViewAt() const;
  
  const glm::vec3 &getUp() const;
  
  float getFY() const;
  
  const glm::mat3 &getMCW();
  
  glm::vec3 view_from_{}; // ray origin or eye or O
  float f_y_{1.0f}; // focal lenght (px)
  glm::vec3 view_at_{}; // target T
private:
  
  int width_{640}; // image width (px)
  int height_{480};  // image height (px)
  float fov_y_{0.785f}; // vertical field of view (rad)
  
  glm::vec3 up_{glm::vec3(0.0f, 0.0f, 1.0f)}; // up vector
  
  glm::mat3 M_c_w_{}; // transformation matrix from CS -> WS
  
};


#endif //RTCAMERA_H
