/*

    Declaração das propiedades da entidade archive.
    
    Autora: Danielys Davila
    Versão: v0.0.1 10/05/2025
    Ultima revisão: 10/05/2025

*/

#ifndef _ARCHIVE_H
#define _ARCHIVE_H

#include <stdio.h>
#include "membro.h"

typedef struct Archive{
    char nome[100];
    FILE *arquivo;
    Membro *membros;
    int num_membros;

} Archive;

#endif