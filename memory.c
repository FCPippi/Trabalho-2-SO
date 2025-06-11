#include "memory.h"
#include <stdio.h>

// mapeia frames fisicos -> paginas virtuais
// -1 = frame livre
int frames[NUM_FRAMES];      

// ordem de alocacao para politica FIFO
int allocation_order[NUM_FRAMES];  

// contador global para IDs FIFO
int next_allocation_id = 0;        

// inicializa a memoria fisica
void init_memory() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        frames[i] = -1;
        allocation_order[i] = -1;
    }
    next_allocation_id = 0;  // reinicia contador FIFO
}

// verifica se a memoria fisica ta cheia
int is_memory_full() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i] == -1) {
            return 0;
        }
    }
    return 1;  // todos frames ocupados
}

int allocate_frame(
    int page,  // pagina virtual q vai ser mapeada
    void (*update_callback)(int old_page, int new_page, int frame, void *data),  // callback para atualizar estruturas especificas
    void *callback_data  // dados extras opcionais para callback
) {
    // verifica se a pagina ta mapeada em algum frame
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i] == page) {
            return i;  // retorna frame existente (evita duplicacao)
        }
    }
    
    // Tenta encontrar um frame livre
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i] == -1) {  // frame livre encontrado
            frames[i] = page;   // mpeia pagina no frame
            // atualiza ordem FIFO com novo ID
            allocation_order[i] = next_allocation_id++;  
            
            // notifica estruturas externas sobre nova alocacao
            if (update_callback) {
                // old_page = -1 (nao havia pagina anterior)
                update_callback(-1, page, i, callback_data);
            }
            return i;  // Retorna indice do frame alocado
        }
    }
    
    int oldest_frame = 0;  // indice do frame mais antigo
    // Encontra frame com menor ID FIFO (mais antigo)
    for (int i = 1; i < NUM_FRAMES; i++) {
        if (allocation_order[i] < allocation_order[oldest_frame]) {
            oldest_frame = i;
        }
    }
    
    // obtem pagina que sera substituida
    int old_page = frames[oldest_frame];  
    
    // Notifica sobre substituicao
    if (update_callback) {
        update_callback(old_page, page, oldest_frame, callback_data); // informa pagina antiga e nova pagina
    }
    
    // atualiza estruturas locais:
    frames[oldest_frame] = page;  // mapeia nova pagina
    allocation_order[oldest_frame] = next_allocation_id++;  // atualiza ID FIFO (agora e o frame mais recente)
    
    return oldest_frame;  // Retorna frame substituido
}
void dump_memory(FILE *out) {
    fprintf(out, "\nEstado da Memoria Fisica:\n");
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i] != -1) {
            fprintf(out, "Moldura %d: Pagina %d (Ordem FIFO: %d)\n", 
                   i, frames[i], allocation_order[i]);
        } else {
            // Frame livre
            fprintf(out, "Moldura %d: Livre\n", i);
        }
    }
}