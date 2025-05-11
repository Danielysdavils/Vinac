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

#define FILE_NAME 100

typedef struct Archive{
    Membro *membros;
    char nome[FILE_NAME];
    FILE *arquivo;
    int num_membros;

} Archive;


int abrir_archive(Archive *arq, const char *nome);

void fechar_arquive(Archive *arq);

int salvar_directorio(Archive *arq);

#endif