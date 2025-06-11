#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Gera endereços virtuais aleatórios ou lê de arquivo existente
 * @param[out] output Ponteiro para array que receberá os endereços
 * @return Quantidade de endereços gerados ou lidos
 * @details Se arquivo entrada.txt existe, lê endereços dele.
 *          Caso contrário, gera endereços aleatórios e salva no arquivo.
 * @warning Aloca memória dinamicamente - caller deve liberar com free()
 * @note Gera 100 endereços por padrão se arquivo não existir
 */
int generate_or_read_input(int **output);

/**
 * @brief Identifica o segmento de memória de um endereço virtual
 * @param vaddr Endereço virtual (0 a VIRTUAL_MEM_SIZE-1)
 * @return String constante com nome do segmento
 * @details Segmentação da memória:
 *          - .text: 0 a TEXT_SIZE-1
 *          - .data: TEXT_SIZE a (TEXT_SIZE + DATA_SIZE - 1)
 *          - .bss:  (TEXT_SIZE + DATA_SIZE) a (TEXT_SIZE + DATA_SIZE + BSS_SIZE - 1)
 *          - .stack: resto do espaço virtual
 */
const char *get_segment_name(int vaddr);

/**
 * @brief Exibe a configuração atual do simulador
 * @details Mostra tamanhos de memória, páginas, molduras e segmentos
 *          Útil para debug e documentação da execução
 */
void print_configuration();

#endif
