#include "utils.h"
#include <stdlib.h>
#include <time.h>

void InitRandom(void) {
    srand((unsigned int)time(NULL));
}

int RandomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

float RandomFloat(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}
