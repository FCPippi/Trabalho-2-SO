#ifndef PAGETABLE_1LVL_H
#define PAGETABLE_1LVL_H

#include <stdio.h>

/**
 * @brief Traduz endereço virtual para físico usando tabela de 1 nível
 * @param vaddr Endereço virtual a ser traduzido (0 a VIRTUAL_MEM_SIZE-1)
 * @return Endereço físico correspondente ou -1 se memória física cheia
 * @details Algoritmo:
 *          1. Calcula página = vaddr / PAGE_SIZE
 *          2. Calcula offset = vaddr % PAGE_SIZE
 *          3. Se página já mapeada, retorna moldura * PAGE_SIZE + offset
 *          4. Senão, aloca nova moldura e mapeia
 */
int translate_1lvl(int vaddr);

/**
 * @brief Gera relatório da tabela de páginas de 1 nível
 * @param out Ponteiro para arquivo onde escrever o relatório
 * @details Lista apenas as páginas que estão mapeadas para molduras
 */
void dump_pagetable_1lvl(FILE *out);

/**
 * @brief Inicializa tabela de páginas de 1 nível
 * @details Marca todas as entradas da tabela como livres (-1)
 */
void init_pagetable_1lvl();

#endif