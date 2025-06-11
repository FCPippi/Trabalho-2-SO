#include "pagetable_inv.h"
#include "config.h"
#include "memory.h"
#include <stdio.h>

typedef struct {
    int pid;
    int page;
} InvertedEntry;

InvertedEntry inverted_table[NUM_FRAMES];

void init_pagetable_inv() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        inverted_table[i].pid = -1;
        inverted_table[i].page = -1;
    }
}

int translate_inverted(int vaddr) {
    int page = vaddr / PAGE_SIZE;
    int offset = vaddr % PAGE_SIZE;

    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i] == page) {
            return i * PAGE_SIZE + offset;
        }
    }

    int frame = allocate_frame(page);
    if (frame == -1) {
        return -1;
    }

    inverted_table[frame].pid = 0;
    inverted_table[frame].page = page;
    return frame * PAGE_SIZE + offset;
}

void dump_pagetable_inv(FILE *out) {
    fprintf(out, "\nTabela de Páginas Invertida:\n");
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (inverted_table[i].page != -1) {
            fprintf(out, "Moldura %d: PID=%d, Página=%d\n", 
                   i, inverted_table[i].pid, inverted_table[i].page);
        }
    }
}
