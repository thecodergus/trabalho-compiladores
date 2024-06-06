#ifndef CONVERSOES_H
#define CONVERSOES_H

#include <stdio.h>
#include <stdlib.h>

#include "libraries/str.h"

float intToFloat(int num);

str intToString(int num);

float stringToFloat(str string);

int stringToInt(str string);

int floatToInt(float num);

str floatToString(float num);

#endif