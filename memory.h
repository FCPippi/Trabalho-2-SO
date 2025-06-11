#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include "config.h"

/**
 * @brief Inicializa a memória física do simulador
 * @details Marca todas as molduras como livres (-1), preparando
 *          o sistema para alocação sob demanda
 */
void init_memory();


/**
 * @brief Aloca uma moldura livre para uma página virtual
 * @param page_number Número da página virtual a ser mapeada
 * @return Índice da moldura alocada (0 a NUM_FRAMES-1) ou -1 se memória cheia
 * @details Percorre o array de molduras procurando uma livre e a marca como ocupada
 * @warning Função falha se todas as molduras estiverem ocupadas
 */
int allocate_frame(int page_number);


/**
 * @brief Gera relatório do estado atual da memória física
 * @param out Ponteiro para arquivo onde escrever o relatório
 * @details Lista o conteúdo de cada moldura (página mapeada ou -1 se livre)
 */
void dump_memory(FILE *out);

/**
 * @brief Verifica se a memória física está completamente ocupada
 * @return 1 se todas as molduras estão ocupadas, 0 se há molduras livres
 * @details Percorre o array de molduras verificando se alguma está livre (-1)
 */
int is_memory_full();

extern int frames[NUM_FRAMES]; // vetor de molduras

#endif