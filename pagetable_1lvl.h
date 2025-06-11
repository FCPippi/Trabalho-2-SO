#ifndef PAGETABLE_1LVL_H
#define PAGETABLE_1LVL_H

#include <stdio.h>

/**
 * @brief Atualiza a tabela de paginas de 1 nivel
 * @param old_page Numero da pagina virtual antiga
 * @param new_page Numero da pagina virtual nova
 * @param frame Indice da moldura fisica alocada
 * @param data Dados adicionais para callback (se necessario)
 * 
 * @details Esta funcao deve ser chamada quando uma pagina virtual e substituida por outra.
 *          Ela atualiza a entrada correspondente na tabela de paginas, garantindo que o mapeamento
 *          esteja correto.
 */
void update_pagetable_1lvl(int old_page, int new_page, int frame, void *data);

/**
 * @brief Traduz endereco virtual para fisico usando Tabela de Paginas de 1 Nivel
 * @param vaddr Endereco virtual a ser traduzido
 * @return Endereco fisico correspondente ou -1 se memoria fisica cheia
 * 
 * @details ALGORITMO DE 1 NiVEL (MAPEAMENTO DIRETO):
 *          Implementacao mais simples e direta de paginacao.
 *          Cada pagina virtual tem uma entrada correspondente na tabela.
 * 
 *          Estrutura: page_table[pagina_virtual] = moldura_fisica
 * 
 *          PASSOS DA TRADUcaO:
 *          1. usa numero da pagina como indice direto na tabela
 *          2. verifica se pagina ja esta mapeada
 *          3. se nao mapeada: aloca moldura e atualiza tabela
 *  
 * @example Para vaddr = 1000:
 *          - page = 1000/256 = 3, offset = 1000%256 = 232
 *          - Verifica page_table[3]: nao mapeada (-1)
 *          - Aloca moldura 0, atualiza: page_table[3] = 0
 *          - Retorna: 0*256 + 232 = 232
 *
 */
int translate_1lvl(int vaddr);

/** 
 * @brief Aloca uma moldura para uma pagina virtual na tabela de 1 nivel
 * @param page Numero da pagina virtual a ser mapeada
 * @return Indice da moldura alocada (0 a NUM_FRAMES-1) ou -1 se memoria cheia
 * @details Percorre o array de molduras procurando uma livre e a marca como ocupada.
*/
int allocate_frame_1lvl(int page);

/**
 * @brief Gera relatorio da tabela de paginas de 1 nivel
 * @param out Ponteiro para arquivo onde escrever o relatorio
 * @details Lista apenas as paginas que estao mapeadas para molduras
 */
void dump_pagetable_1lvl(FILE *out);

/**
 * @brief Inicializa tabela de paginas de 1 nivel
 * @details Marca todas as entradas da tabela como livres (-1)
 */
void init_pagetable_1lvl();

#endif