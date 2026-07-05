#ifndef SPAWNSCSV_H
#define SPAWNSCSV_H

#include <stddef.h>

typedef struct {
    unsigned int guid;
    unsigned int creatureID;
    unsigned int map;
    float x;
    float y;
    float z;
    float orientation;
    unsigned int zoneID;
    unsigned int areaID;
} SpawnRecord;

typedef struct {
    SpawnRecord *items;
    size_t count;
    size_t capacity;
} SpawnArray;

int loadSpawns(const char *path, SpawnArray *out);
void freeSpawnArr(SpawnArray *arr);

#endif