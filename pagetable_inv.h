#ifndef PAGETABLE_INV_H
#define PAGETABLE_INV_H

#include <stdio.h>

/**
 * @brief Traduz endereco virtual para fisico usando Tabela de Paginas Invertida
 * @param vaddr Endereco virtual a ser traduzido (0 a VIRTUAL_MEM_SIZE-1)
 * @return Endereco fisico correspondente ou -1 se memoria fisica cheia
 * 
 * @details A tabela invertida e indexada pelas molduras fisicas (nao pelas paginas virtuais).
 *          Cada entrada da tabela contem informacões sobre qual pagina virtual esta
 *          armazenada em cada moldura fisica.
 * 
 *          Estrutura: inverted_table[moldura] = {pid, pagina_virtual}
 * 
 *          PASSOS DA TRADUcaO:
 *          1. extrai numero da pagina e offset do endereco virtual
 *          2. percorre todas as molduras procurando a pagina
 *          3. se encontrada: calcula endereco fisico diretamente
 *          4. se nao encontrada: aloca nova moldura e atualiza tabela invertida
 * 
 * @example Para vaddr = 1000:
 *          - page = 1000/256 = 3, offset = 1000%256 = 232
 *          - busca pagina 3 nas molduras: nao encontrada
 *          - aloca moldura 0, atualiza: inverted_table[0] = {pid=0, page=3}
 *          - retorna: 0*256 + 232 = 232
 */
int translate_inverted(int vaddr);

/**
 * @brief Aloca uma moldura para uma pagina virtual na tabela de paginas invertida
 * @param page Numero da pagina virtual a ser mapeada
 * @return Indice da moldura alocada (0 a NUM_FRAMES-1) ou -1 se memoria cheia
 * 
 */
void update_pagetable_inv(int old_page, int new_page, int frame, void *data);

/**
 * @brief Gera relatorio da tabela de paginas invertida
 * @param out Ponteiro para arquivo onde escrever o relatorio
 * @details Lista molduras ocupadas com seus respectivos PID e numero da pagina
 */
void dump_pagetable_inv(FILE *out);

/**
 * @brief Inicializa tabela de paginas invertida
 * @details Marca todas as entradas como livres (pid = -1, page = -1)
 */
void init_pagetable_inv();

#endif