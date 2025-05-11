#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "library/lz/lz.h"
#include "entidades/actions.h"
#include "entidades/archive.h"
#include "entidades/membro.h"


/*************************************************************************
*                           INTERNAL FUNCTIONS                           *
*************************************************************************/

Membro create_membro(const char *path_membro)
{
    struct stat st;
    stat(path_membro, &st);

    Membro novo;

    strcpy(novo.nome, path_membro);
    novo.uid = getuid();
    novo.tamanho_original = st.st_size;
    novo.tamanho_disco = st.st_size;
    novo.data_modificacao = st.st_mtime;
    novo.ordem = 0;
    novo.comprimido = 0;
    novo.offset = 0;

    return novo;
}

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

    char buffer[TAM_BUFFER];
    size_t lidos;
    while((lidos = fread(buffer, 1, sizeof(buffer), orig)) > 0)
        fwrite(buffer, 1, lidos, tmp);

    fclose(orig);
    fclose(tmp);
    remove(arq->nome);
    rename("tmp.vc", arq->nome);
}

int inserir_membro(Archive *arq, const char *path_membro)
{
    FILE *m = fopen(path_membro, "rb");
    if(!m)
        return 0;

    Membro novo = create_membro(path_membro);
    novo.ordem = arq->num_membros;

    fseek(arq->arquivo, 0, SEEK_END);
    novo.offset = ftell(arq->arquivo);
    
    char buffer[TAM_BUFFER];
    size_t lidos;
    while((lidos = fread(buffer, 1, sizeof(buffer), m)) > 0)
    {
        fwrite(buffer, 1, lidos, arq->arquivo);
    }

    fclose(m);

    arq->membros = realloc(arq->membros, sizeof(Membro) * (arq->num_membros + 1));
    arq->membros[arq->num_membros] = novo;
    arq->num_membros++;

    return 1;
}

int inserir_membro_comprimido(Archive *arq, const char *path_membro)
{
    FILE *m = fopen(path_membro, "rb");
    if(!m)
        return 0;

    Membro novo = create_membro(path_membro);
 
    unsigned char *buffer_original = malloc(novo.tamanho_original);
    if(!buffer_original)
    {
        fclose(m);
        return 0;
    }

    fread(buffer_original, 1, novo.tamanho_original, m);
    fclose(m);

    size_t tamaho_max_comprimido = novo.tamanho_original * 2;
    unsigned char *buffer_comprimido = malloc(tamaho_max_comprimido);
    if(!buffer_comprimido){
        free(buffer_original);
        return 0;
    }

    int tamanho_comprimido = LZ_Compress(buffer_original, buffer_comprimido, novo.tamanho_original);
    if(tamanho_comprimido <= 0)
    {
        free(buffer_original);
        free(buffer_comprimido);
        return 0;
    }

    novo.tamanho_disco = tamanho_comprimido;
    novo.ordem = arq->num_membros;
    novo.comprimido = 1;

    fseek(arq->arquivo, 0, SEEK_END);
    novo.offset = ftell(arq->arquivo);

    fwrite(buffer_comprimido, 1, tamanho_comprimido, arq->arquivo);

    arq->membros = realloc(arq->membros, sizeof(Membro) * (arq->num_membros + 1));
    arq->membros[arq->num_membros] = novo;
    arq->num_membros++;

    free(buffer_original);
    free(buffer_comprimido);
    
    return 1;
}



/*************************************************************************
*                           EXTERNAL FUNCTIONS                           *
*************************************************************************/

void inserir_semCompressao(int argc, char *argv[])
{
    printf("VINAC [INSERT-NOCOMP]");

    Archive arq;
    if(!abrir_archive(&arq, argv[2]))
    {
        perror("VINAC [INSERT-NOCOM]: ERROR ao abrir o arquivo");
        return 1; 
    }

    for(int i = 3; i < argc; i++){
        if(!inserir_membro(&arq, argv[i]))
            fprintf(strerror, "VINAC [INSERT-NOCOM]: Erro ao inserir membro %s\n", argv[i]);
    }

    salvar_directorio(&arq);
    fechar_arquive(&arq);

    return 0;
}