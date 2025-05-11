#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#include "membro.h"


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