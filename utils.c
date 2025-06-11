#include "utils.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_or_read_input(int **output) {
    FILE *f = fopen("entrada.txt", "r");
    int count = 100;
    *output = malloc(sizeof(int) * count);

    if (!*output) {
        printf("Erro: nao foi possivel alocar memoria\n");
        return 0;
    }

    if (f) {
        printf("Lendo enderecos do arquivo entrada.txt...\n");
        int actual_count = 0;
        for (int i = 0; i < count; i++) {
            if (fscanf(f, "%d", &(*output)[i]) != 1) {
                break;
            }
            actual_count++;
        }
        fclose(f);
        printf("Lidos %d enderecos do arquivo\n", actual_count);
        return actual_count > 0 ? actual_count : count;
    } else {
        printf("Arquivo entrada.txt nao encontrado. Gerando enderecos aleatorios...\n");
        f = fopen("entrada.txt", "w");
        if (!f) {
            printf("Erro: nao foi possivel criar o arquivo entrada.txt\n");
            return 0;
        }
        srand(time(NULL));
        for (int i = 0; i < count; i++) {
            (*output)[i] = rand() % VIRTUAL_MEM_SIZE;
            fprintf(f, "%d\n", (*output)[i]);
            printf("Gerado endereco %d: %d\n", i+1, (*output)[i]);
        }
        fclose(f);
        printf("Enderecos salvos em entrada.txt\n");
    }
    return count;
}

const char *get_segment_name(int vaddr) {
    if (vaddr < TEXT_SIZE) {
        return ".text";
    } else if (vaddr < TEXT_SIZE + DATA_SIZE) {
        return ".data";
    } else if (vaddr < TEXT_SIZE + DATA_SIZE + BSS_SIZE) {
        return ".bss";
    } else {
        return ".stack";
    }
}

void print_configuration() {
    printf("=== Configuracao do Simulador ===\n");
    printf("Tamanho do espaco virtual: %d bits (%d bytes)\n", VIRTUAL_BITS, VIRTUAL_MEM_SIZE);
    printf("Tamanho da memoria fisica: %d bits (%d bytes)\n", PHYSICAL_BITS, PHYSICAL_MEM_SIZE);
    printf("Tamanho da pagina: %d bits (%d bytes)\n", PAGE_BITS, PAGE_SIZE);
    printf("Numero de paginas: %d\n", NUM_PAGES);
    printf("Numero de molduras: %d\n", NUM_FRAMES);
    printf("\nTamanhos dos segmentos:\n");
    printf(".text: %d bytes\n", TEXT_SIZE);
    printf(".data: %d bytes\n", DATA_SIZE);
    printf(".bss: %d bytes\n", BSS_SIZE);
    printf(".stack: %d bytes\n", STACK_SIZE);
    printf("===================================\n\n");
}
