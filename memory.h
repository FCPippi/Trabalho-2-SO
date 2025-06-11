#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include "config.h"

/**
 * @brief Inicializa a memoria fisica do simulador
 * @details Marca todas as molduras como livres (-1), preparando
 *          o sistema para alocacao sob demanda
 */
void init_memory();

/**
 * @brief Gera relatorio do estado atual da memoria fisica
 * @param out Ponteiro para arquivo onde escrever o relatorio
 * @details Lista o conteudo de cada moldura (pagina mapeada ou -1 se livre)
 */
void dump_memory(FILE *out);

/**
 * @brief Verifica se a memoria fisica esta completamente ocupada
 * @return 1 se todas as molduras estao ocupadas, 0 se ha molduras livres
 * @details Percorre o array de molduras verificando se alguma esta livre (-1)
 */
int is_memory_full();

/**
 * @brief Aloca uma moldura fisica para uma pagina virtual
 * @param page Numero da pagina virtual a ser mapeada
 * @param update_callback Funcao de callback para atualizar estruturas
 * @param callback_data Dados adicionais para o callback
 * @return Indice da moldura alocada (0 a NUM_FRAMES-1) ou -1 se memoria cheia
 * @details Percorre o array de molduras procurando uma livre. Se a pagina ja estiver mapeada,
 *          chama o callback com os dados antigos e novos. Se nao, aloca uma nova moldura ou substitui uma existente.
 */
int allocate_frame(int page, void (*update_callback)(int old_page, int new_page, int frame, void *data), void *callback_data);

extern int frames[NUM_FRAMES]; // vetor de molduras

#endif