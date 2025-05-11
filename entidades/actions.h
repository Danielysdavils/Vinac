/*

    Declaração de ações do vinac.
    Por ações defino funcionalidades do vinac usadas no main.
    
    Autora: Danielys Davila
    Versão: v0.0.1 10/05/2025
    Ultima revisão: 10/05/2025

*/

#ifndef _ACTIONS_H
#define _ACTIONS

#define TAM_BUFFER 4096

/*
    Insere um ou varios membros no archive sem compressão
    Caso o membro já exista no archive, ele é substituido.
*/
int inserir_semCompressao(int argc, char *argv[]);

/*
    Insere um ou varios membros no archive com compressão
    Caso o membro já exista ele é substituido.
*/
int inserir_compressao(int argc, char *argv[]);

/*
    Movimenta um membro indicado na linha de comando para
    imediatamente depois do membro target existente em archive.
    Caso target NUll, movimentação no inicio. 
*/
int mover_membro(int argc, char *argv[]);

/*
    Extrai os membros indicados no archive.
    Se nehum membro for especificado no parametro extrai todos os archivos.
*/
int extrair(int argc, char *argv[]);

/*
    Remove os membros indicados no archive.
    Se nenhum membro for especificado no parametro remove todos os membros do archive.
*/
int remover(int argc, char *argv[]);

/*
    Lista o conteúdo do archive em ordem
*/
int listar(int argc, char *argv[]);


#endif