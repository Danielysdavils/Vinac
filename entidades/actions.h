/*

    Declaração de ações do vinac.
    Por ações defino funcionalidades do vinac usadas no main.
    
    Autora: Danielys Davila
    Versão: v0.0.1 10/05/2025
    Ultima revisão: 10/05/2025

*/

#ifndef _ACTIONS_H
#define _ACTIONS

#include <stdbool.h>

#define TAM_BUFFER 4096

/*
    Insere um ou varios membros no archive sem compressão
    Caso o membro já exista no archive, ele é substituido.
*/
bool inserir_semCompressao(int argc, char *argv[]);

/*
    Insere um ou varios membros no archive com compressão
    Caso o membro já exista ele é substituido.
*/
bool inserir_compressao(int argc, char *argv[]);

/*
    Movimenta um membro indicado na linha de comando para
    imediatamente depois do membro target existente em archive.
    Caso target NUll, movimentação no inicio. 
*/
bool mover_membro(int argc, char *argv[]);

/*
    Extrai os membros indicados no archive.
    Se nehum membro for especificado no parametro extrai todos os archivos.
*/
bool extrair_membros(int argc, char *argv[]);

/*
    Remove os membros indicados no archive.
    Se nenhum membro for especificado no parametro remove todos os membros do archive.
*/
bool remover(int argc, char *argv[]);

/*
    Lista o conteúdo do archive em ordem
*/
bool listar(int argc, char *argv[]);


#endif