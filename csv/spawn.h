#ifndef SPAWN_H
#define SPAWN_H

typedef struct SpawnRecord {
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

#endif
