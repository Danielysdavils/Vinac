#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "entidades/archive.h"

struct archive *carregar_arquivo(const char *nome_arquivo) 
{
    struct archive *arq = create_archive();
    if(!arq)
    {
        printf("error alocate arq");
        return NULL;
    }

    strcpy(arq->nome, nome_arquivo);
    arq->arquivo = fopen(nome_arquivo, "rb");
    if (!arq->arquivo) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome_arquivo);
        free(arq);
        return NULL;
    }

    // Lê o número de membros
    if (fread(&arq->num_membros, sizeof(int), 1, arq->arquivo) != 1) {
        fprintf(stderr, "Erro ao ler o número de membros no arquivo\n");
        fclose(arq->arquivo);
        free(arq);
        return NULL;
    }

    // Aloca memória para armazenar os membros
    arq->membros = malloc(arq->num_membros * sizeof(struct membro));
    if (!arq->membros) {
        fprintf(stderr, "Erro ao alocar memória para os membros\n");
        fclose(arq->arquivo);
        free(arq);
        return NULL;
    }

    // Lê os dados dos membros
    if (fread(arq->membros, sizeof(struct membro), arq->num_membros, arq->arquivo) != (long unsigned int)arq->num_membros) {
        fprintf(stderr, "Erro ao ler os dados dos membros no arquivo\n");
        fclose(arq->arquivo);
        free(arq->membros);
        free(arq);
        return NULL;
    }

    return arq;
}

bool abrir_archive(struct archive *arq, const char *nome)
{
    strcpy(arq->nome, nome);
    arq->arquivo = fopen(nome, "ab");
    
    if(!arq->arquivo)
    {
        return false;
    }
         
    arq->membros = NULL;
    arq->num_membros = 0;

    return true;
}

struct archive *create_archive()
{
    struct archive *arq = malloc(sizeof(struct archive));
    if (!arq) {
        printf("Erro ao criar novo arquivo de archive.\n");
        return NULL;
    }

    arq->num_membros = 0;
    arq->membros = NULL;
    arq->arquivo = fopen("archive.bin", "wb");
    if (!arq->arquivo) {
        printf("Erro ao criar o arquivo de archive.\n");
        free(arq);
        return NULL;
    }

    // Inicializa o arquivo com os dados iniciais (se necessário)
    fwrite(&arq->num_membros, sizeof(int), 1, arq->arquivo);
    return arq;
}

void fechar_arquive(struct archive *arq)
{
    if(arq->arquivo)
        fclose(arq->arquivo);

    if(arq->membros)
        free(arq->membros);
}

bool salvar_directorio(struct archive *arq)
{
    FILE *tmp = fopen("tmp.vc", "wb");
    if(!tmp) 
        return false;

    // vamos calcular os novos offsets
    long offset_atual = sizeof(int) + sizeof(struct membro) * arq->num_membros;
    
    for(int i = 0; i < arq->num_membros; i++) {
        arq->membros[i].offset = offset_atual;
        offset_atual += arq->membros[i].tamanho_disco;
    }

    fwrite(&arq->num_membros, sizeof(int), 1, tmp);

    for(int i = 0; i < arq->num_membros; i++)
        fwrite(&arq->membros[i], sizeof(struct membro), 1, tmp);
    
    for(int i = 0; i < arq->num_membros; i++) {
        FILE *m = fopen(arq->membros[i].nome, "rb");
        if(!m) continue;

        char buffer[4096];
        size_t lidos;
        while((lidos = fread(buffer, 1, sizeof(buffer), m)) > 0)
            fwrite(buffer, 1, lidos, tmp);

        fclose(m);
    }

    fclose(tmp);
    remove(arq->nome);
    rename("tmp.vc", arq->nome);

    return true;
}