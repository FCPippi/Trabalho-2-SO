#include "pagetable_1lvl.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

int page_table[NUM_PAGES];

void init_pagetable_1lvl() {
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i] = -1;  // -1 = pagina nao mapeada
    }
}

// Callback para atualizar estruturas especificas da tabela de 1 nivel
void update_pagetable_1lvl(int old_page, int new_page, int frame, void *data) {
    // Se ha uma pagina antiga, remove o mapeamento
    if (old_page != -1) {
        page_table[old_page] = -1;
    }
    
    // Mapeia a nova pagina
    page_table[new_page] = frame;
}

int translate_1lvl(int vaddr) {
    int page = vaddr / PAGE_SIZE;
    int offset = vaddr % PAGE_SIZE;
    
    // verifica se a pagina ta mapeada
    if (page_table[page] != -1) {
        return page_table[page] * PAGE_SIZE + offset;
    }
    
    // Usa o metodo unificado de alocacao
    int frame = allocate_frame(page, update_pagetable_1lvl, NULL);
    if (frame == -1) {
        return -1;
    }
    
    return frame * PAGE_SIZE + offset;
}

void dump_pagetable_1lvl(FILE *out) {
    fprintf(out, "\n=== Tabela de 1 Nivel ====\n");
    fprintf(out, "Estado das Paginas Mapeadas:\n");
    
    int mapped_count = 0;
    for (int i = 0; i < NUM_PAGES; i++) {
        if (page_table[i] != -1) {
            fprintf(out, "Pagina %4d -> Moldura %2d\n", i, page_table[i]);
            mapped_count++;
        }
    }
    
    fprintf(out, "Total de paginas mapeadas: %d\n", mapped_count);
}