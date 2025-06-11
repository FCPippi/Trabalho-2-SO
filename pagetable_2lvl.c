#include "pagetable_2lvl.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

int *directory[DIR_SIZE] = {NULL};

void init_pagetable_2lvl() {
    for (int i = 0; i < DIR_SIZE; i++) {
        directory[i] = NULL;
    }
}

// Callback para atualizar estruturas especificas da tabela de 2 niveis
void update_pagetable_2lvl(int old_page, int new_page, int frame, void *data) {
    // Se ha uma pagina antiga, remove o mapeamento
    if (old_page != -1) {
        int old_dir = old_page / TABLE_SIZE;
        int old_tbl = old_page % TABLE_SIZE;
        if (directory[old_dir] != NULL) {
            directory[old_dir][old_tbl] = -1;
        }
    }
    
    // Mapeia a nova pagina
    int new_dir = new_page / TABLE_SIZE;
    int new_tbl = new_page % TABLE_SIZE;
    
    // Aloca o diretorio se nao existir
    if (directory[new_dir] == NULL) {
        directory[new_dir] = calloc(TABLE_SIZE, sizeof(int));
        if (directory[new_dir] == NULL) {
            return; // Erro de alocacao
        }
        for (int i = 0; i < TABLE_SIZE; i++) {
            directory[new_dir][i] = -1;
        }
    }
    
    directory[new_dir][new_tbl] = frame;
}

int translate_2lvl(int vaddr) {
    int page = vaddr / PAGE_SIZE;
    int offset = vaddr % PAGE_SIZE;
    int dir_index = page / TABLE_SIZE;
    int tbl_index = page % TABLE_SIZE;
    
    // Verifica se o diretorio existe e se a pagina ja esta mapeada
    if (directory[dir_index] != NULL && directory[dir_index][tbl_index] != -1) {
        return directory[dir_index][tbl_index] * PAGE_SIZE + offset;
    }
    
    // Usa o metodo unificado de alocacao
    int frame = allocate_frame(page, update_pagetable_2lvl, NULL);
    if (frame == -1) {
        return -1;
    }
    
    return frame * PAGE_SIZE + offset;
}

void dump_pagetable_2lvl(FILE *out) {
    fprintf(out, "\n=== Tabela de 2 Niveis ===\n");
    
    fprintf(out, "\nTabelas de Paginas Alocadas:\n");
    int tables_allocated = 0;
    int total_mapped = 0;
    
    for (int i = 0; i < DIR_SIZE; i++) {
        if (directory[i] != NULL) {
            tables_allocated++;
            fprintf(out, "Diretorio %2d: ", i);
            int mapped_in_table = 0;
            for (int j = 0; j < TABLE_SIZE; j++) {
                if (directory[i][j] != -1) {
                    fprintf(out, "[%d->%d] ", i*TABLE_SIZE+j, directory[i][j]);
                    mapped_in_table++;
                    total_mapped++;
                }
            }
            if (mapped_in_table == 0) {
                fprintf(out, "(vazia)");
            }
            fprintf(out, "\n");
        }
    }
    
    fprintf(out, "Total de Tabelas Alocadas: %d/%d\n", tables_allocated, DIR_SIZE);
    fprintf(out, "Total de paginas mapeadas: %d\n", total_mapped);
}