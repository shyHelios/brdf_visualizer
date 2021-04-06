/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef UTILS_H
#define UTILS_H

#define SAFE_DELETE(p) { \
  if ( p != NULL ) \
  { \
    delete p; \
    p = NULL; \
  } \
}

#define SAFE_DELETE_ARRAY(p) { \
  if ( p != NULL ) \
  { \
    delete [] p; \
    p = NULL; \
  } \
}

inline long long GetFileSize64(const char *file_name) {
  FILE *file = fopen(file_name, "rb");
  
  if (file != NULL) {
    fseeko64(file, 0, SEEK_END); // přesun na konec souboru
    long long file_size = ftello64(file); // zjištění aktuální pozice
    fseeko64(file, 0, SEEK_SET); // přesun zpět na začátek
    fclose(file);
    
    return file_size;
  }
  
  return 0;
}


char *LTrim(char *s) {
  while (isspace(*s) || (*s == 0)) {
    ++s;
  }
  
  return s;
}

char *RTrim(char *s) {
  char *back = s + strlen(s);
  
  while (isspace(*--back));
  
  *(back + 1) = '\0';
  
  return s;
}

char *Trim(char *s) {
  return RTrim(LTrim(s));
}

#endif //UTILS_H
