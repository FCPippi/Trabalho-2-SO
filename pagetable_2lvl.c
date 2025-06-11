#include "pagetable_2lvl.h"
#include "config.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

#define DIR_SIZE 16
#define TABLE_SIZE (NUM_PAGES / DIR_SIZE)

int *directory[DIR_SIZE];

void init_pagetable_2lvl() {
    for (int i = 0; i < DIR_SIZE; i++) {
        directory[i] = NULL;
    }
}

int translate_2lvl(int vaddr) {
    int page = vaddr / PAGE_SIZE;
    int offset = vaddr % PAGE_SIZE;
    int dir_index = page / TABLE_SIZE;
    int tbl_index = page % TABLE_SIZE;

    // verifica se o diretorio existe
    if (!directory[dir_index]) {
        directory[dir_index] = malloc(TABLE_SIZE * sizeof(int));
        for (int i = 0; i < TABLE_SIZE; i++) {
            directory[dir_index][i] = -1;
        }
    }

    // verifica se a tabela ta mapeada
    if (directory[dir_index][tbl_index] != -1) {
        return directory[dir_index][tbl_index] * PAGE_SIZE + offset;
    }

    int frame = allocate_frame(page);
    if (frame == -1) {
        return -1; 
    }
    directory[dir_index][tbl_index] = frame;

    return frame * PAGE_SIZE + offset;
}

void dump_pagetable_2lvl(FILE *out) {
    fprintf(out, "\nTabela de Páginas (2 níveis):\n");
    for (int i = 0; i < DIR_SIZE; i++) {
        if (directory[i]) {
            for (int j = 0; j < TABLE_SIZE; j++) {
                if (directory[i][j] != -1) {
                    int page = i * TABLE_SIZE + j;
                    fprintf(out, "Página %d -> Moldura %d\n", page, directory[i][j]);
                }
            }
        }
    }
}