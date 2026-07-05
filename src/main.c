#include <stdio.h>
#include "spawnsCSV.h"

int main(int argc, char **argv) {
    if (argc != 2){
        fprintf(stderr, "Incorrect arguments\nUsage: %s <file>.csv\n", argv[0]);
        return 1;
    }

    SpawnArray spawns;

    if (loadSpawns(argv[1], &spawns) != 0){return 1;}
    printf("Loaded %zu creature spawn defs\n", spawns.count);
    return 0;
}