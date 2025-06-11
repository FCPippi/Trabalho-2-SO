#ifndef PAGETABLE_2LVL_H
#define PAGETABLE_2LVL_H

#include <stdio.h>

/**
 * @brief Traduz endereço virtual para físico usando tabela de 2 níveis
 * @param vaddr Endereço virtual a ser traduzido
 * @return Endereço físico correspondente ou -1 se memória física cheia
 * @details Algoritmo:
 *          1. Calcula índices do diretório e tabela interna
 *          2. Cria tabela interna se ainda não existe
 *          3. Verifica se página já está mapeada
 *          4. Aloca moldura se necessário
 *          5. Retorna endereço físico
 * @note Aloca tabelas internas dinamicamente conforme necessário
 */
int translate_2lvl(int vaddr);

/**
 * @brief Gera relatório da tabela de páginas de 2 níveis
 * @param out Ponteiro para arquivo onde escrever o relatório
 * @details Percorre o diretório e todas as tabelas internas, listando mapeamentos
 */
void dump_pagetable_2lvl(FILE *out);

/**
 * @brief Inicializa tabela de páginas de 2 níveis
 * @details Inicializa o diretório de tabelas com todos os ponteiros NULL
 */
void init_pagetable_2lvl();

#endif