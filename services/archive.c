#include <string.h>
#include <stdlib.h>

#include "archive.h"


int abrir_archive(Archive *arq, const char *nome)
{
    strcpy(arq->nome, nome);
    arq->arquivo = fopen(nome, "ab+");
    
    if(!arq->arquivo)
        return 0;

    arq->membros = NULL;
    arq->num_membros = 0;

    return 1;
}

void fechar_arquive(Archive *arq)
{
    if(arq->arquivo)
        fclose(arq->arquivo);

    if(arq->membros)
        free(arq->membros);
}

int salvar_directorio(Archive *arq)
{
    FILE *tmp = fopen("tmp.vc", "wb");
    if(!tmp) return 0;

    fwrite(&arq->num_membros, sizeof(int), 1, tmp);

    for(int i = 0; i < arq->num_membros; i++)
    {
        fwrite(&arq->membros[i], sizeof(Membro), 1, tmp);
    }

    FILE *orig = fopen(arq->nome, "rb");
    fseek(orig, sizeof(int) + sizeof(Membro) * arq->num_membros, SEEK_SET);

    char buffer[4096];
    size_t lidos;
    while((lidos = fread(buffer, 1, sizeof(buffer), orig)) > 0)
        fwrite(buffer, 1, lidos, tmp);

    fclose(orig);
    fclose(tmp);
    remove(arq->nome);
    rename("tmp.vc", arq->nome);

    return 1;
}