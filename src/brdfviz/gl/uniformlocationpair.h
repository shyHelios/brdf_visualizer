/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef UNIFORMLOCATIONPAIR_H
#define UNIFORMLOCATIONPAIR_H

template<typename T_DATA_TYPE>
struct UniformLocationPair : public std::pair<int, T_DATA_TYPE> {
  int &getUniformLocation() {
    return this->first;
  }
  
  T_DATA_TYPE &getData() {
    return this->second;
  }
};


#endif //UNIFORMLOCATIONPAIR_H
