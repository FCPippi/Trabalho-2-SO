#include "pagetable_1lvl.h"
#include "config.h"
#include "memory.h"
#include <stdio.h>

int page_table[NUM_PAGES];


void init_pagetable_1lvl() {
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i] = -1;  // -1 = livre
    }
}

int translate_1lvl(int vaddr) {
    int page = vaddr / PAGE_SIZE;
    int offset = vaddr % PAGE_SIZE;

    // verifica se ta mapeada
    if (page_table[page] != -1) {
        return page_table[page] * PAGE_SIZE + offset;
    }

    // aloca moldura
    int frame = allocate_frame(page);
    if (frame == -1) {
        return -1;
    }
    page_table[page] = frame;

    return frame * PAGE_SIZE + offset;
}

void dump_pagetable_1lvl(FILE *out) {
    fprintf(out, "\nTabela de Páginas (1 nível):\n");
    for (int i = 0; i < NUM_PAGES; i++) {
        if (page_table[i] != -1) {
            fprintf(out, "Página %d -> Moldura %d\n", i, page_table[i]);
        }
    }
}