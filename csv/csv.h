#ifndef CSV_H
#define CSV_H

#include <stddef.h>
#include "spawn.h"

typedef struct SpawnArray {
    SpawnRecord *items;
    size_t count;
    size_t capacity;
} SpawnArray;

int loadSpawnCSV(const char *path, SpawnArray *out);
void freeSpawnArr(SpawnArray *arr);

#endif
