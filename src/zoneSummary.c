#include "zoneSummary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ZoneSummary {
    unsigned int zoneID;
    unsigned int mmask;
    size_t spawnCnt;
    float minx;
    float maxx;
    float miny;
    float maxy;
    unsigned int minLvl;
} ZoneSummary;

static int zoneCmpSummary(const void *a, const void *b) {
    const ZoneSummary *za = (const ZoneSummary *)a;
    const ZoneSummary *zb = (const ZoneSummary *)b;

    if (za->zoneID < zb->zoneID) return -1;
    if (za->zoneID > zb->zoneID) return 1;
    return 0;
}

static int zoneCmpCntSummary(const void *a, const void *b) {
    const ZoneSummary *za = (const ZoneSummary *)a;
    const ZoneSummary *zb = (const ZoneSummary *)b;

    if (za->spawnCnt > zb->spawnCnt) return -1;
    if (za->spawnCnt < zb->spawnCnt) return 1;
    if (za->zoneID < zb->zoneID) return -1;
    if (za->zoneID > zb->zoneID) return 1;
    return 0;
}

static ZoneSummary *findZone(ZoneSummary *zones, size_t count, unsigned int zoneID) {
    for (size_t i = 0; i < count; i++) {
        if (zones[i].zoneID == zoneID) {
            return &zones[i];
        }
    }
    return NULL;
}

static void outMapList(unsigned int mmask) {
    int first = 1;
    for (unsigned int map = 0; map < 32; map++) {
        if ((mmask & (1u << map)) != 0) {
            if (!first) {
                printf("/");
            }
            printf("%u", map);
            first = 0;
        }
    }
}

void outZoneList(const SpawnArray *spawns) {
    ZoneSummary *zones = NULL;
    size_t zoneCnt = 0;
    size_t zoneCap = 0;

    for (size_t i = 0; i < spawns->count; i++) {
        const SpawnRecord *s = &spawns->items[i];
        ZoneSummary *zone = findZone(zones, zoneCnt, s->zoneID);

        if (!zone) {
            if (zoneCnt == zoneCap) {
                size_t newCap = zoneCap == 0 ? 128 : zoneCap * 2;
                ZoneSummary *newZones = realloc(zones, newCap * sizeof(ZoneSummary));
                if (!newZones) {
                    fprintf(stderr, "out of memory building zone summary\n");
                    free(zones);
                    return;
                }
                zones = newZones;
                zoneCap = newCap;
            }

            zone = &zones[zoneCnt++];
            memset(zone, 0, sizeof(*zone));
            zone->zoneID = s->zoneID;
            zone->spawnCnt = 0;
            zone->minx = zone->maxx = s->x;
            zone->miny = zone->maxy = s->y;
        }

        zone->spawnCnt++;
        if (s->map < 32) {
            zone->mmask |= (1u << s->map);
        }
        if (s->x < zone->minx) zone->minx = s->x;
        if (s->x > zone->maxx) zone->maxx = s->x;
        if (s->y < zone->miny) zone->miny = s->y;
        if (s->y > zone->maxy) zone->maxy = s->y;
    }

    qsort(zones, zoneCnt, sizeof(ZoneSummary), zoneCmpCntSummary);

    printf("Total spawns: %zu\n", spawns->count);
    printf("Unique zones: %zu\n\n", zoneCnt);

    printf("%-8s %-10s %-8s %-24s %-24s\n", "zoneID", "spawns", "maps", "x_range", "y_range");
    printf("%-8s %-10s %-8s %-24s %-24s\n", "-------", "------", "----", "-------", "-------");

    for (size_t i = 0; i < zoneCnt; i++) {
        char x_range[64];
        char y_range[64];
        snprintf(x_range, sizeof(x_range), "%.1f to %.1f", zones[i].minx, zones[i].maxx);
        snprintf(y_range, sizeof(y_range), "%.1f to %.1f", zones[i].miny, zones[i].maxy);

        printf("%-8u %-10zu ", zones[i].zoneID, zones[i].spawnCnt);
        outMapList(zones[i].mmask);
        printf("%*s", 8 - 1, "");
        printf("%-24s %-24s\n", x_range, y_range);
    }

    qsort(zones, zoneCnt, sizeof(ZoneSummary), zoneCmpSummary);
    free(zones);
}
