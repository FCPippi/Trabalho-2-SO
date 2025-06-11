#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Gera enderecos virtuais aleatorios ou le de arquivo existente
 * @param[out] output Ponteiro para array que recebera os enderecos
 * @return Quantidade de enderecos gerados ou lidos
 * @details Se arquivo entrada.txt existe, le enderecos dele.
 *          Caso contrario, gera enderecos aleatorios e salva no arquivo.
 * @warning Aloca memoria dinamicamente - caller deve liberar com free()
 * @note Gera 100 enderecos por padrao se arquivo nao existir
 */
int generate_or_read_input(int **output);

/**
 * @brief Identifica o segmento de memoria de um endereco virtual
 * @param vaddr Endereco virtual (0 a VIRTUAL_MEM_SIZE-1)
 * @return String constante com nome do segmento
 * @details Segmentacao da memoria:
 *          - .text: 0 a TEXT_SIZE-1
 *          - .data: TEXT_SIZE a (TEXT_SIZE + DATA_SIZE - 1)
 *          - .bss:  (TEXT_SIZE + DATA_SIZE) a (TEXT_SIZE + DATA_SIZE + BSS_SIZE - 1)
 *          - .stack: resto do espaco virtual
 */
const char *get_segment_name(int vaddr);

/**
 * @brief Exibe a configuracao atual do simulador
 * @details Mostra tamanhos de memoria, paginas, molduras e segmentos
 *          util para debug e documentacao da execucao
 */
void print_configuration();

#endif
