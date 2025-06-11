#include "memory.h"
#include <stdio.h>

int frames[NUM_FRAMES];

void init_memory() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        frames[i] = -1;  // -1 = moldura livre
    }
}

int allocate_frame(int page_number) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i] == -1) {
            frames[i] = page_number;
            printf("    Alocando moldura %d para página %d\n", i, page_number);
            return i;
        }
    }
    printf("    ERRO: Todas as %d molduras estão ocupadas!\n", NUM_FRAMES);
    printf("    Estado atual das molduras: ");
    for (int i = 0; i < NUM_FRAMES; i++) {
        printf("[%d]=%d ", i, frames[i]);
    }
    printf("\n");
    return -1;
}

int is_memory_full() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i] == -1) {
            return 0;
        }
    }
    return 1;
}

void dump_memory(FILE *out) {
    fprintf(out, "\nEstado da Memoria Fisica:\n");
    for (int i = 0; i < NUM_FRAMES; i++) {
        fprintf(out, "Moldura %d: %d\n", i, frames[i]);
    }
}
