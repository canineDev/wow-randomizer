#include "csv.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 1024

static int ensureCap(SpawnArray *arr) {
    if (arr->count < arr->capacity) {
        return 0;
    }

    size_t newCap = arr->capacity == 0 ? 4096 : arr->capacity * 2;
    SpawnRecord *newItems = realloc(arr->items, newCap * sizeof(SpawnRecord));
    if (!newItems) {
        return -1;
    }

    arr->items = newItems;
    arr->capacity = newCap;
    return 0;
}

static int parseSpawnLine(char *line, SpawnRecord *record) {
    char *fields[9];
    int field_count = 0;

    char *token = strtok(line, ",\n\r");
    while (token && field_count < 9) {
        fields[field_count++] = token;
        token = strtok(NULL, ",\n\r");
    }

    if (field_count != 9) {
        return -1;
    }

    record->guid = (unsigned int)strtoul(fields[0], NULL, 10);
    record->creatureID = (unsigned int)strtoul(fields[1], NULL, 10);
    record->map = (unsigned int)strtoul(fields[2], NULL, 10);
    record->x = strtof(fields[3], NULL);
    record->y = strtof(fields[4], NULL);
    record->z = strtof(fields[5], NULL);
    record->orientation = strtof(fields[6], NULL);
    record->zoneID = (unsigned int)strtoul(fields[7], NULL, 10);
    record->areaID = (unsigned int)strtoul(fields[8], NULL, 10);

    return 0;
}

int loadSpawnCSV(const char *path, SpawnArray *out) {
    memset(out, 0, sizeof(*out));

    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open '%s': %s\n", path, strerror(errno));
        return -1;
    }

    char line[LINE_BUFFER_SIZE];
    unsigned long lineNumber = 0;
    unsigned long skippedLines = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        if (lineNumber == 1) {continue;}
        if (line[0] == '\0' || line[0] == '\n' || line[0] == '\r') {continue;}

        if (ensureCap(out) != 0) {
            fprintf(stderr, "Out of memory while reading CSV.\n");
            fclose(file);
            freeSpawnArr(out);
            return -1;
        }

        SpawnRecord record;
        if (parseSpawnLine(line, &record) != 0) {
            skippedLines++;
            continue;
        }

        out->items[out->count++] = record;
    }

    fclose(file);
    if (skippedLines > 0) {fprintf(stderr, "Warning: skipped %lu malformed CSV lines.\n", skippedLines);}
    return 0;
}

void freeSpawnArr(SpawnArray *arr) {
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}
