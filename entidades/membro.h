/*

    Declaração das propiedades da entidade membro.
    
    Autora: Danielys Davila
    Versão: v0.0.1 10/05/2025
    Ultima revisão: 10/05/2025

*/

#ifndef _MEMBRO_H
#define _MEMBRO_H

#include <time.h>

typedef struct{
    char nome[100];
    int uid;
    long tamanho_original;
    long tamanho_disco;
    time_t data_modificacao;
    long offset;
    int comprimido;
    int ordem;
    
} Membro;

#endif