#include "csv.h"
#include "zoneSummary.h"

#include <stdio.h>

int main(int argc, char **argv) {
    const char *csvPath = "randomizer_creature_zones.csv";
    
    if (argc >= 2) {csvPath = argv[1];}
    printf("Loading spawn zone export: %s\n\n\n", csvPath);
    SpawnArray spawns;
    if (loadSpawnCSV(csvPath, &spawns) != 0) {return 1;}
    outZoneList(&spawns);
    freeSpawnArr(&spawns);

    return 0;
}
