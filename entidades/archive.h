/*

    Declaração das propiedades da entidade archive.
    
    Autora: Danielys Davila
    Versão: v0.0.1 10/05/2025
    Ultima revisão: 10/05/2025

*/

#ifndef _ARCHIVE_H
#define _ARCHIVE_H

#include <stdio.h>
#include <stdbool.h>

#include "membro.h"

#define FILE_NAME 100

struct archive
{
    struct membro *membros;
    char nome[FILE_NAME];
    FILE *arquivo;
    int num_membros;
};


struct archive *carregar_arquivo(const char *nome_arquivo);

struct archive *create_archive();

bool abrir_archive(struct archive *arq, const char *nome);

void fechar_arquive(struct archive *arq);

bool salvar_directorio(struct archive *arq);


#endif