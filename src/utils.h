#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// Random number generation
int RandomInt(int min, int max);
float RandomFloat(float min, float max);

// Initialize random seed
void InitRandom(void);

#endif // UTILS_H
