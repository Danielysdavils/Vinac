/*

    Declaração das propiedades da entidade membro.
    
    Autora: Danielys Davila
    Versão: v0.0.1 10/05/2025
    Ultima revisão: 10/05/2025

*/

#ifndef _MEMBRO_H
#define _MEMBRO_H

#include <time.h>

#define Membro_NAME 100

typedef struct{
    char nome[Membro_NAME];
    int uid;
    long tamanho_original;
    long tamanho_disco;
    time_t data_modificacao;
    long offset;
    int comprimido;
    int ordem;
    
} Membro;


Membro create_membro(const char *path_membro);

#endif