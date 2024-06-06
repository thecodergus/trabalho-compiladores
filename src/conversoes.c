#include "conversoes.h"

float intToFloat(int num) { return (float)num; }

str intToString(int num) {
  char* str = malloc((int)((ceil(log10(num)) + 1) * sizeof(char)));

  sprintf(str, "%d", num);

  return str_acquire(str);
}

float stringToFloat(str string) { return atof(str_ptr(string)); }

int stringToInt(str string) { return atoi(str_ptr(string)); }

int floatToInt(float num) { return (int)num; }

str floatToString(float num) {
  char* str = malloc((int)((ceil(log10(num)) + 1) * sizeof(char)));

  sprintf(str, "%f", num);

  return str_acquire(str);
}