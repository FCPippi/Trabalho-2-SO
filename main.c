#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include "config.h"
#include "memory.h"
#include "utils.h"
#include "pagetable_1lvl.h"
#include "pagetable_2lvl.h"
#include "pagetable_inv.h"

PagingType paging_type = PAGETABLE_1LVL;

/**
 * @brief Gera um relatorio completo no arquivo de saida
 * @param out Arquivo de saida
 * @param vaddr Endereco virtual processado
 * @param paddr Endereco fisico resultante
 * @param processed_addresses Contador de enderecos processados
 */
void generate_full_report(FILE *out, int vaddr, int paddr, int processed_addresses) {
    const char *segment = get_segment_name(vaddr);
    
    // Endereco traduzido
    fprintf(out, "Endereco Virtual: %d (segmento %s) -> Endereco Fisico: %d\n", 
            vaddr, segment, paddr);
    
    // Estado atual
    fprintf(out, "\n=== Estado Atual ===\n");
    
    // Tabela de paginas
    switch (paging_type) {
        case PAGETABLE_1LVL: dump_pagetable_1lvl(out); break;
        case PAGETABLE_2LVL: dump_pagetable_2lvl(out); break;
        case PAGETABLE_INVERTED: dump_pagetable_inv(out); break;
    }
    
    // Memoria fisica
    dump_memory(out);
    
    // Estatisticas
    int used_frames = 0;
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i] != -1) used_frames++;
    }
    
    fprintf(out, "\n=== Estatisticas ===\n");
    fprintf(out, "Enderecos processados: %d\n", processed_addresses);
    fprintf(out, "Molduras utilizadas: %d/%d\n", used_frames, NUM_FRAMES);
    fprintf(out, "Taxa de ocupacao: %.2f%%\n", (float)used_frames/NUM_FRAMES*100);
    
    // Separador para o proximo relatorio
    fprintf(out, "\n====-----------------NOVO-CONTEUDO---------------------------===\n");
    fflush(out);
}

int main(int argc, char *argv[]) {
    printf("=== Simulador de Memoria Paginada ===\n");

    if (argc < 2) {
        printf("Uso: %s [1|2|inv] [r|i]\n", argv[0]);
        printf("  1: tabela de paginas de 1 nivel\n");
        printf("  2: tabela de paginas de 2 niveis\n");
        printf("  inv: tabela de paginas invertida\n");
        printf("  r: gerar 10 enderecos aleatorios\n");
        printf("  i: modo interativo (padrao)\n");
        return 1;
    }

    // Configura tipo de paginacao
    if (strcmp(argv[1], "1") == 0) {
        paging_type = PAGETABLE_1LVL;
        printf("Usando tabela de paginas de 1 nivel\n");
    } else if (strcmp(argv[1], "2") == 0) {
        paging_type = PAGETABLE_2LVL;
        printf("Usando tabela de paginas de 2 niveis\n");
    } else if (strcmp(argv[1], "inv") == 0) {
        paging_type = PAGETABLE_INVERTED;
        printf("Usando tabela de paginas invertida\n");
    } else {
        printf("Tipo de tabela invalido. Use 1, 2 ou inv.\n");
        return 1;
    }

    // Verifica se deve usar modo aleatorio ou interativo
    int random_mode = 0;
    if (argc > 2 && strcmp(argv[2], "r") == 0) {
        random_mode = 1;
        printf("Modo aleatorio ativado. Gerando 10 enderecos.\n");
    } else {
        printf("Modo interativo ativado. Digite enderecos virtuais.\n");
    }

    if (VIRTUAL_MEM_SIZE < PHYSICAL_MEM_SIZE) {
        printf("Erro: Espaco virtual deve ser >= memoria fisica\n");
        return 1;
    }

    print_configuration();

    init_memory();
    switch (paging_type) {
        case PAGETABLE_1LVL: init_pagetable_1lvl(); break;
        case PAGETABLE_2LVL: init_pagetable_2lvl(); break;
        case PAGETABLE_INVERTED: init_pagetable_inv(); break;
    }

    FILE *out = fopen("saida.txt", "w");
    fprintf(out, "=== Simulador de Memoria Paginada ===\n");
    fprintf(out, "Tipo de paginacao: %s\n", 
           paging_type == PAGETABLE_1LVL ? "1 nivel" : 
           paging_type == PAGETABLE_2LVL ? "2 niveis" : "Invertida");
    fprintf(out, "=====================================\n\n");

    int processed_addresses = 0;
    int memory_full = 0; // Flag para controle de memoria cheia
    
    if (random_mode) {
        // Modo aleatorio - gera 100 enderecos
        srand(time(NULL));
        for (int i = 0; i < 100; i++) {
            // Verifica se a memoria esta cheia antes de processar
            if (is_memory_full()) {
                printf("Memoria fisica cheia! Processamento interrompido.\n");
                fprintf(out, "\n=== ATENCAO: Memoria fisica cheia! Processamento interrompido. ===\n");
                memory_full = 1;
                break;
            }
            
            int vaddr = rand() % VIRTUAL_MEM_SIZE;
            printf("\nProcessando endereco aleatorio %d: %d\n", i+1, vaddr);
            
            int paddr = -1;
            
            switch (paging_type) {
                case PAGETABLE_1LVL: paddr = translate_1lvl(vaddr); break;
                case PAGETABLE_2LVL: paddr = translate_2lvl(vaddr); break;
                case PAGETABLE_INVERTED: paddr = translate_inverted(vaddr); break;
            }

            if (paddr == -1) {
                printf("Memoria fisica cheia! Nao foi possivel mapear o endereco %d\n", vaddr);
                fprintf(out, "\n=== ATENCAO: Memoria fisica cheia! ===\n");
                memory_full = 1;
                break;
            }

            printf("  -> Endereco fisico: %d\n", paddr);
            processed_addresses++;

            // Gera relatorio completo para este endereco
            generate_full_report(out, vaddr, paddr, processed_addresses);
        }
    } else {
        // Modo interativo
        printf("\nDigite enderecos virtuais para traduzir (ou 'q' para sair):\n");
        char input[20];
        
        while (1) {
            // Verifica se a memoria esta cheia antes de processar
            if (is_memory_full()) {
                printf("Memoria fisica cheia! Nao e possivel processar mais enderecos.\n");
                fprintf(out, "\n=== ATENCAO: Memoria fisica cheia! Processamento interrompido. ===\n");
                memory_full = 1;
                break;
            }
            
            printf("\nEndereco virtual (0-%d): ", VIRTUAL_MEM_SIZE-1);
            fgets(input, sizeof(input), stdin);
            
            input[strcspn(input, "\n")] = 0;
            if (tolower(input[0]) == 'q') break;
            
            int vaddr = atoi(input);
            if (vaddr < 0 || vaddr >= VIRTUAL_MEM_SIZE) {
                printf("Endereco invalido! Deve estar entre 0 e %d\n", VIRTUAL_MEM_SIZE-1);
                continue;
            }

            int paddr = -1;
            
            switch (paging_type) {
                case PAGETABLE_1LVL: paddr = translate_1lvl(vaddr); break;
                case PAGETABLE_2LVL: paddr = translate_2lvl(vaddr); break;
                case PAGETABLE_INVERTED: paddr = translate_inverted(vaddr); break;
            }

            if (paddr == -1) {
                printf("Memoria fisica cheia! Nao foi possivel mapear o endereco %d\n", vaddr);
                fprintf(out, "\n=== ATENCAO: Memoria fisica cheia! ===\n");
                memory_full = 1;
                break;
            }

            printf("  -> Endereco fisico: %d\n", paddr);
            processed_addresses++;

            // Gera relatorio completo para este endereco
            generate_full_report(out, vaddr, paddr, processed_addresses);
        }
    }

    // Adicione um relatorio final quando a memoria encher
    if (memory_full) {
        fprintf(out, "\n=== RELATORIO FINAL (MEMORIA CHEIA) ===\n");
        fprintf(out, "Total de enderecos processados: %d\n", processed_addresses);
        
        int used_frames = 0;
        for (int i = 0; i < NUM_FRAMES; i++) {
            if (frames[i] != -1) used_frames++;
        }
        
        fprintf(out, "Molduras utilizadas: %d/%d\n", used_frames, NUM_FRAMES);
        fprintf(out, "Taxa de ocupacao: %.2f%%\n", (float)used_frames/NUM_FRAMES*100);
    }

    fclose(out);
    printf("\nResultados salvos em saida.txt\n");
    return 0;
}