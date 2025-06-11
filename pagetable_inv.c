#include "pagetable_inv.h"
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

// Callback para atualizar tabela invertida durante alocacoes
void update_pagetable_inv(
    int old_page,  // pagina sendo substituida
    int new_page,  // nova pagina a mapear
    int frame,     // frame fisico afetado
    void *data     // nao utilizado
) {
    inverted_table[frame].pid = 0;   // so um processo
    inverted_table[frame].page = new_page;  // nova pagina mapeada
}

int translate_inverted(int vaddr) {
    int page = vaddr / PAGE_SIZE;     // numero da pagina
    int offset = vaddr % PAGE_SIZE;   // deslocamento
    
    // procura pagina na tabela invertida
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (inverted_table[i].page == page) {
            // calcula endereco fisico: frame * tamanho da pagina + offset
            return i * PAGE_SIZE + offset;
        }
    }
    
    // Page fault
    int frame = allocate_frame(
        page,                // pagina alocar
        update_pagetable_inv, // callback para atualizar tabela
        NULL                 
    );
    // memoria cheia
    if (frame == -1) {
        return -1;
    }
    
    // Retorna endereco fisico calculado
    return frame * PAGE_SIZE + offset;
}

void dump_pagetable_inv(FILE *out) {
    fprintf(out, "\n=== Tabela Invertida ===\n");
    fprintf(out, "Estado das Molduras:\n");
    
    int mapped_count = 0;
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (inverted_table[i].page != -1) {
            fprintf(out, "Moldura %2d: PID=%d, Pagina=%d\n", 
                   i, inverted_table[i].pid, inverted_table[i].page);
            mapped_count++;
        } else {
            fprintf(out, "Moldura %2d: Livre\n", i);
        }
    }
    
    fprintf(out, "Total de molduras ocupadas: %d/%d\n", mapped_count, NUM_FRAMES);
}