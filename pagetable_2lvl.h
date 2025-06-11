#ifndef PAGETABLE_2LVL_H
#define PAGETABLE_2LVL_H

#include <stdio.h>

#define DIR_SIZE 16
#define TABLE_SIZE (NUM_PAGES / DIR_SIZE)

/**
 * @brief Atualiza a tabela de paginas de 2 niveis
 * @param old_page Numero da pagina virtual antiga (-1 se nao houver)
 * @param new_page Numero da pagina virtual nova
 * @param frame Indice da moldura fisica alocada
 * 
 * @details Remove o mapeamento da pagina antiga (se existir) e adiciona o novo mapeamento.
 *          Se o diretorio nao existir, cria uma nova tabela interna.
 */
void update_pagetable_2lvl(int old_page, int new_page, int frame, void *data);

/**
 * @brief Traduz endereco virtual para fisico usando Tabela de Paginas de 2 Niveis
 * @param vaddr Endereco virtual a ser traduzido
 * @return Endereco fisico correspondente ou -1 se memoria fisica cheia
 * 
 * @details  Divide o numero da pagina em dois indices para navegacao hierarquica:
 *          - indice do diretorio (nivel superior)
 *          - indice da tabela interna (nivel inferior)
 * 
 *          Estrutura: directory[dir_index] -> tabela_interna[tbl_index] -> moldura
 * 
 *          PASSOS DA TRADUcaO:
 *          1. calcula indices hierarquicos a partir do numero da pagina
 *          2. verifica/cria tabela interna no diretorio
 *          3. verifica se pagina ja esta mapeada na tabela interna
 *          4. se nao mapeada: aloca moldura e atualiza tabela interna
 * 
 * @example Para vaddr = 1500 (assumindo TABLE_SIZE = 16):
 *          - page = 1500/256 = 5, offset = 1500%256 = 220
 *          - dir_index = 5/16 = 0, tbl_index = 5%16 = 5
 *          - Acessa directory[0][5], encontra moldura 1
 *          - Retorna: 1*256 + 220 = 476
 */
int translate_2lvl(int vaddr);

/**
 * @brief Aloca uma moldura para uma pagina virtual na tabela de 2 niveis
 * @param page Numero da pagina virtual a ser mapeada
 * @return Indice da moldura alocada (0 a NUM_FRAMES-1) ou -1 se memoria cheia
 * @details Percorre o array de molduras procurando uma livre. Se a pagina ja estiver mapeada,
 *          atualiza o bit de referencia. Se nao, aloca uma nova moldura ou substitui uma existente
 *          usando o algoritmo LRU.
 */
int allocate_frame_2lvl(int page);

/**
 * @brief Gera relatorio da tabela de paginas de 2 niveis
 * @param out Ponteiro para arquivo onde escrever o relatorio
 * @details Percorre o diretorio e todas as tabelas internas, listando mapeamentos
 */
void dump_pagetable_2lvl(FILE *out);

/**
 * @brief Inicializa tabela de paginas de 2 niveis
 * @details Inicializa o diretorio de tabelas com todos os ponteiros NULL
 */
void init_pagetable_2lvl();

#endif