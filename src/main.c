#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 2){
        fprintf(stderr, "Incorrect arguments\nUsage: %s <file>.csv\n", argv[0]);
        return 1;
    }
    return 0;
}