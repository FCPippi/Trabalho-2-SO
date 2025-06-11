#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "memory.h"
#include "utils.h"
#include "pagetable_1lvl.h"
#include "pagetable_2lvl.h"
#include "pagetable_inv.h"

PagingType paging_type = PAGETABLE_1LVL;

/**
 * @brief Função principal do simulador de memória paginada
 * @param argc Número de argumentos da linha de comando
 * @param argv Array de argumentos da linha de comando
 * @return 0 se execução bem-sucedida, 1 em caso de erro
 * @details Controla todo o fluxo do simulador:
 *          1. Valida argumentos e define tipo de paginação
 *          2. Inicializa estruturas de dados
 *          3. Gera ou lê endereços de entrada
 *          4. Processa cada endereço virtual
 *          5. Gera arquivo de saída com resultados
 * @note Uso: programa [1|2|inv] onde:
 *       - 1: tabela de 1 nível
 *       - 2: tabela de 2 níveis
 *       - inv: tabela invertida
 */
int main(int argc, char *argv[]) {
    printf("=== Simulador de Memória Paginada ===\n");
    printf("Diretório atual: %s\n", getcwd(NULL, 0) ? getcwd(NULL, 0) : "desconhecido");
    
    // Validação de argumentos
    if (argc < 2) {
        printf("Uso: %s [1|2|inv]\n", argv[0]);
        printf("  1   - Tabela de páginas de 1 nível\n");
        printf("  2   - Tabela de páginas de 2 níveis\n");
        printf("  inv - Tabela de páginas invertida\n");
        return 1;
    }

    // Configuração do tipo de paginação
    if (strcmp(argv[1], "1") == 0) {
        paging_type = PAGETABLE_1LVL;
        printf("Usando tabela de páginas de 1 nível\n");
    } else if (strcmp(argv[1], "2") == 0) {
        paging_type = PAGETABLE_2LVL;
        printf("Usando tabela de páginas de 2 níveis\n");
    } else if (strcmp(argv[1], "inv") == 0) {
        paging_type = PAGETABLE_INVERTED;
        printf("Usando tabela de páginas invertida\n");
    } else {
        printf("Tipo de tabela inválido. Use 1, 2 ou inv.\n");
        return 1;
    }

    // Validação da configuração
    if (VIRTUAL_MEM_SIZE < PHYSICAL_MEM_SIZE) {
        printf("Erro: Espaço virtual deve ser >= memória física\n");
        return 1;
    }

    print_configuration();

    // Inicialização das estruturas
    init_memory();
    switch (paging_type) {
        case PAGETABLE_1LVL:
            init_pagetable_1lvl();
            break;
        case PAGETABLE_2LVL:
            init_pagetable_2lvl();
            break;
        case PAGETABLE_INVERTED:
            init_pagetable_inv();
            break;
    }

    // Geração ou leitura de endereços
    int *virtual_addresses = NULL;
    int count = generate_or_read_input(&virtual_addresses);

    printf("Processando %d endereços virtuais...\n", count);

    // Abertura do arquivo de saída
    FILE *out = fopen("saida.txt", "w");
    if (!out) {
        printf("Erro ao criar arquivo de saída saida.txt\n");
        printf("Verifique as permissões do diretório atual\n");
        free(virtual_addresses);
        return 1;
    }

    printf("Arquivo de saída saida.txt criado com sucesso\n");

    // Cabeçalho do arquivo de saída
    fprintf(out, "=== Simulador de Memória Paginada ===\n");
    fprintf(out, "Tipo de paginação: ");
    switch (paging_type) {
        case PAGETABLE_1LVL:
            fprintf(out, "1 nível\n");
            break;
        case PAGETABLE_2LVL:
            fprintf(out, "2 níveis\n");
            break;
        case PAGETABLE_INVERTED:
            fprintf(out, "Invertida\n");
            break;
    }
    fprintf(out, "=====================================\n\n");

    // Processamento dos endereços virtuais
    int memory_full = 0;
    int processed_addresses = 0;
    for (int i = 0; i < count && !memory_full; i++) {
        processed_addresses = i + 1;
        int vaddr = virtual_addresses[i];
        int paddr = -1;
        const char *segment = get_segment_name(vaddr);

        printf("Processando endereço %d: %d (página %d)\n", i+1, vaddr, vaddr/PAGE_SIZE);
        
        // Tradução baseada no tipo de paginação
        switch (paging_type) {
            case PAGETABLE_1LVL:
                paddr = translate_1lvl(vaddr);
                break;
            case PAGETABLE_2LVL:
                paddr = translate_2lvl(vaddr);
                break;
            case PAGETABLE_INVERTED:
                paddr = translate_inverted(vaddr);
                break;
        }

        // Tratamento de memória cheia
        if (paddr == -1) {
            fprintf(out, "ERRO: Memória física cheia. Encerrando simulação.\n");
            printf("Memória física cheia no endereço %d (página %d). Simulação encerrada.\n", vaddr, vaddr/PAGE_SIZE);
            memory_full = 1;
            processed_addresses--;
            break;
        }

        printf("  -> Endereço físico: %d\n", paddr);
        fprintf(out, "Endereço Virtual: %d (segmento %s) -> Endereço Físico: %d\n", 
                vaddr, segment, paddr);
    }

    // Estado final das estruturas
    fprintf(out, "\n=== Estado Final das Estruturas ===\n");
    
    switch (paging_type) {
        case PAGETABLE_1LVL:
            dump_pagetable_1lvl(out);
            break;
        case PAGETABLE_2LVL:
            dump_pagetable_2lvl(out);
            break;
        case PAGETABLE_INVERTED:
            dump_pagetable_inv(out);
            break;
    }

    dump_memory(out);

    // Cálculo de estatísticas
    int used_frames = 0;
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i] != -1) used_frames++;
    }
    
    fprintf(out, "\n=== Estatísticas ===\n");
    fprintf(out, "Endereços processados: %d\n", processed_addresses);
    fprintf(out, "Molduras utilizadas: %d/%d\n", used_frames, NUM_FRAMES);
    fprintf(out, "Taxa de ocupação: %.2f%%\n", (float)used_frames / NUM_FRAMES * 100);

    // Limpeza de recursos
    free(virtual_addresses);
    fclose(out);

    printf("Simulação concluída com sucesso!\n");
    printf("Arquivos criados:\n");
    printf("  - entrada.txt (endereços de entrada)\n");
    printf("  - saida.txt (resultados da simulação)\n");
    return 0;
}
