#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#include "actions.h"

typedef bool (*vinac_action)();
typedef struct {
    const char *param;
    vinac_action action;

} VinacAction;

VinacAction vinacActions[] = {
    {"-ip", inserir_semCompressao},
    {"-p",  inserir_semCompressao},
    {"-ic", inserir_compressao},
    {"-i",  inserir_compressao},
    {"-m",  mover_membro},
    {"-x",  extrair_membros},
    {"-r",  remover},
    {"-c",  listar},
    {NULL,  NULL}
};

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Uso: %s archive.vc membro1 membro2 ... \n", argv[0]);
        return 1;
    }

    const char *param = argv[1];
    for(int i = 0; vinacActions[i].param != NULL; i++)
    {
        if(!strcmp(param, vinacActions[i].param))
        {
            return vinacActions[i].action(argc, argv);
        }
    }

    printf("Comando desconhecido: %s\n", param);
    return 1;
}