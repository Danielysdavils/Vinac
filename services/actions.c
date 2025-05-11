#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lz.h"
#include "actions.h"
#include "archive.h"
#include "membro.h"


/*************************************************************************
*                           INTERNAL FUNCTIONS                           *
*************************************************************************/

int inserir_membro(Archive *arq, const char *path_membro)
{
    FILE *m = fopen(path_membro, "rb");
    if(!m)
        return 0;

    Membro novo = create_membro(path_membro);
    novo.ordem = arq->num_membros;

    fseek(arq->arquivo, 0, SEEK_END);
    novo.offset = ftell(arq->arquivo);
    
    char buffer[4096];
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

    //Caso a o tamanho do arqiuvo comprimido seja maior q o original
    //insere sem compressão
    if(tamanho_comprimido >= novo.tamanho_original)
    {
        free(buffer_original);
        free(buffer_comprimido);

        return inserir_membro(arq, path_membro);
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

int inserir_semCompressao(int argc, char *argv[])
{
    printf("VINAC MODE: [INSERT-NOCOMP]\n");

    Archive arq;
    if(!abrir_archive(&arq, argv[2]))
    {
        perror("[INSERT-NOCOM]: ERROR ao abrir o arquivo\n");
        return 1; 
    }

    for(int i = 3; i < argc; i++){
        if(!inserir_membro(&arq, argv[i]))
            fprintf(stderr, "[INSERT-NOCOM]: Erro ao inserir membro %s\n", argv[i]);
    }

    salvar_directorio(&arq);
    fechar_arquive(&arq);

    printf("[INSERT-NOCOMP]: SUCCESS! \n");
    return 0;
}

int inserir_compressao(int argc, char *argv[])
{
    printf("[INSERT-COMP]\n");

    Archive arq;
    if(!abrir_archive(&arq, argv[2]))
    {
        perror("[INSERT-NOCOM]: ERROR ao abrir o arquivo\n");
        return 1;
    }

    for(int i = 3; i < argc; i++){
        if(!inserir_membro_comprimido(&arq, argv[i]))
            fprintf(stderr, "VINAC [INSERT-NOCOM]: Erro ao inserir membro %s\n", argv[i]);
 
    }

    salvar_directorio(&arq);
    fechar_arquive(&arq);

    return 0;
}

int mover_membro(int argc, char *argv[])
{
    printf("%d", argc);
    printf("%s", argv[0]);
    return 0;
}

int extrair(int argc, char *argv[])
{
    printf("%d", argc);
    printf("%s", argv[0]);
    return 0;
}

int remover(int argc, char *argv[])
{
    printf("%d", argc);
    printf("%s", argv[0]);
    return 0;
}

int listar(int argc, char *argv[])
{
    printf("%d", argc);
    printf("%s", argv[0]);
    return 0;
}