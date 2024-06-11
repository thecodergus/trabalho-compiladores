#ifndef C9D0F935_612F_473F_B0E8_8AE4D136EF3D
#define C9D0F935_612F_473F_B0E8_8AE4D136EF3D

#include <stdio.h>

#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"

void exibir_warning(const char *msg);

void exibir_erro(const char *msg);

#endif /* C9D0F935_612F_473F_B0E8_8AE4D136EF3D */
