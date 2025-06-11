#ifndef PAGETABLE_INV_H
#define PAGETABLE_INV_H

#include <stdio.h>

/**
 * @brief Traduz endereço virtual para físico usando tabela invertida
 * @param vaddr Endereço virtual a ser traduzido
 * @return Endereço físico correspondente ou -1 se memória física cheia
 * @details Algoritmo:
 *          1. Busca a página nas molduras já ocupadas
 *          2. Se encontrada, calcula endereço físico
 *          3. Se não encontrada, aloca nova moldura
 *          4. Atualiza tabela invertida com informações da página
 */
int translate_inverted(int vaddr);

/**
 * @brief Gera relatório da tabela de páginas invertida
 * @param out Ponteiro para arquivo onde escrever o relatório
 * @details Lista molduras ocupadas com seus respectivos PID e número da página
 */
void dump_pagetable_inv(FILE *out);

/**
 * @brief Inicializa tabela de páginas invertida
 * @details Marca todas as entradas como livres (pid = -1, page = -1)
 */
void init_pagetable_inv();

#endif