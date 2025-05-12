#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "entidades/membro.h"

struct membro *create_membro(const char *path_membro)
{
    struct stat st;
    if (stat(path_membro, &st) != 0) {
        fprintf(stderr, "Erro ao obter informações do arquivo '%s'\n", path_membro);
        return NULL; 
    }

    struct membro *novo = malloc(sizeof(struct membro));
    if(!novo) return NULL;

    memset(novo, 0, sizeof(struct membro));

    strncpy(novo->nome, path_membro, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    
    novo->uid = getuid();
    novo->tamanho_original = st.st_size;
    novo->tamanho_disco = st.st_size;
    novo->data_modificacao = st.st_mtime;
    novo->ordem = 0;
    novo->comprimido = false;
    novo->offset = 0;

    return novo;
}