#include "spawnsCSV.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 1024

static int checkCap(SpawnArray *arr) {
    if (arr->count < arr->capacity) {return 0;}

    size_t new_capacity = arr->capacity == 0 ? 4096 : arr->capacity * 2;
    SpawnRecord *new_items = realloc(arr->items, new_capacity * sizeof(SpawnRecord));

    if (!new_items) {return -1;}

    arr->items = new_items;
    arr->capacity = new_capacity;
    return 0;
}

static int parseSpawnLn(char *line, SpawnRecord *rec) {
    char *fields[9];
    int fieldCnt = 0;

    char *token = strtok(line, ",\n\r");

    while (token && fieldCnt < 9) {
        fields[fieldCnt++] = token;
        token = strtok(NULL, ",\n\r");
    }

    if (fieldCnt != 9) {return -1;}

    rec->guid = (unsigned int)strtoul(fields[0], NULL, 10);
    rec->creatureID = (unsigned int)strtoul(fields[1], NULL, 10);
    rec->map = (unsigned int)strtoul(fields[2], NULL, 10);
    rec->x = strtof(fields[3], NULL);
    rec->y = strtof(fields[4], NULL);
    rec->z = strtof(fields[5], NULL);
    rec->orientation = strtof(fields[6], NULL);
    rec->zoneID = (unsigned int)strtoul(fields[7], NULL, 10);
    rec->areaID = (unsigned int)strtoul(fields[8], NULL, 10);

    return 0;
}

int loadSpawns(const char *path, SpawnArray *out) {
    memset(out, 0, sizeof(*out));

    FILE *file = fopen(path, "r");

    if (!file) {
        fprintf(stderr, "Failed to open '%s': %s\n", path, strerror(errno));
        return -1;
    }

    char line[LINE_BUFFER_SIZE];
    unsigned long lineNum = 0;
    unsigned long lineSkip = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNum++;

        if (lineNum == 1) {continue;}
        if (line[0] == '\0' || line[0] == '\n' || line[0] == '\r') {continue;}

        if (checkCap(out) != 0) {
            fprintf(stderr, "Out of memory while reading CSV.\n");
            fclose(file);
            freeSpawnArr(out);
            return -1;
        }

        SpawnRecord rec;

        if (parseSpawnLn(line, &rec) != 0) {
            lineSkip++;
            continue;
        }

        out->items[out->count++] = rec;
    }

    fclose(file);

    if (lineSkip > 0) {
        fprintf(stderr, "Warning: skipped %lu malformed CSV lines.\n", lineSkip);
    }

    return 0;
}

void freeSpawnArr(SpawnArray *arr) {
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}