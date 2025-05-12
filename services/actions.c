#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lz.h"
#include "entidades/actions.h"
#include "entidades/archive.h"
#include "entidades/membro.h"


/*************************************************************************
*                           INTERNAL FUNCTIONS                           *
*************************************************************************/

bool inserir_membro(struct archive *arq, const char *path_membro)
{
     // Verifica se o arquivo já existe, caso contrário cria um novo arquivo de archive
    if (!arq) {
        arq = create_archive();  
        if (!arq) {
            printf("Erro ao criar novo arquivo de archive.\n");
            return false;
        }
    }

    FILE *m = fopen(path_membro, "rb");
    if (!m) {
        printf("Erro ao abrir o arquivo %s.\n", path_membro);
        return false;
    }

    // Cria um novo membro a partir do arquivo
    struct membro *novo = create_membro(path_membro);
    if (!novo) {
        fclose(m);
        return false;
    }

    unsigned char *buffer_original = malloc(novo->tamanho_original);
    if (!buffer_original) {
        printf("Erro ao alocar buffer original.\n");
        fclose(m);
        free(novo);
        return false;
    }

    // Lê o conteúdo original do arquivo
    long int lidos = fread(buffer_original, 1, novo->tamanho_original, m);
    if (lidos != novo->tamanho_original) {
        printf("Erro: fread leu %zu bytes, esperado %ld\n", lidos, novo->tamanho_original);
        free(buffer_original);
        fclose(m);
        free(novo);
        return false;
    }

    fclose(m);

    novo->tamanho_disco = novo->tamanho_original; 
    novo->ordem = arq->num_membros;
    novo->comprimido = false;  

    // Calcula o offset no arquivo
    fseek(arq->arquivo, 0, SEEK_END);
    novo->offset = ftell(arq->arquivo);

    // Grava os dados originais no arquivo
    fwrite(buffer_original, 1, novo->tamanho_original, arq->arquivo);

    // Adiciona o novo membro no array de membros
    arq->membros = realloc(arq->membros, sizeof(struct membro) * (arq->num_membros + 1));
    if (!arq->membros) {
        printf("Erro ao realocar memória para os membros.\n");
        free(buffer_original);
        free(novo);
        return false;
    }

    arq->membros[arq->num_membros] = *novo;
    arq->num_membros++;

    // Libera a memória
    free(novo);
    free(buffer_original);

    // Reabre o arquivo para atualizar a quantidade de membros e os dados
    rewind(arq->arquivo);
    fwrite(&arq->num_membros, sizeof(int), 1, arq->arquivo);
    fwrite(arq->membros, sizeof(struct membro), arq->num_membros, arq->arquivo);

    return true;
}

bool inserir_membro_comprimido(struct archive *arq, const char *path_membro)
{
    // Verifica se o arquivo já existe, caso contrário cria um novo arquivo de archive
    if (!arq) {
        arq = create_archive();  
        if (!arq) {
            printf("Erro ao criar novo arquivo de archive.\n");
            return false;
        }
    }

    FILE *m = fopen(path_membro, "rb");
    if (!m) {
        printf("Erro ao abrir o arquivo %s.\n", path_membro);
        return false;
    }

    // Cria um novo membro a partir do arquivo
    struct membro *novo = create_membro(path_membro);
    if (!novo) {
        fclose(m);
        return false;
    }

    unsigned char *buffer_original = malloc(novo->tamanho_original);
    if (!buffer_original) {
        printf("Erro ao alocar buffer original.\n");
        fclose(m);
        free(novo);
        return false;
    }

    // Lê o conteúdo original do arquivo
    long int lidos = fread(buffer_original, 1, novo->tamanho_original, m);
    if (lidos != novo->tamanho_original) {
        printf("Erro: fread leu %zu bytes, esperado %ld\n", lidos, novo->tamanho_original);
        free(buffer_original);
        fclose(m);
        free(novo);
        return false;
    }

    fclose(m);

    // Estima o tamanho máximo do buffer comprimido
    size_t tamaho_max_comprimido = novo->tamanho_original + (novo->tamanho_original / 250) + 1;
    unsigned char *buffer_comprimido = malloc(tamaho_max_comprimido);
    if (!buffer_comprimido) {
        printf("Erro ao alocar buffer comprimido.\n");
        free(buffer_original);
        free(novo);
        return false;
    }

    memset(buffer_comprimido, 0, tamaho_max_comprimido);

    // Comprime os dados
    int tamanho_comprimido = LZ_Compress(buffer_original, buffer_comprimido, novo->tamanho_original);
    if (tamanho_comprimido <= 0) {
        printf("Erro na compressão.\n");
        free(buffer_original);
        free(buffer_comprimido);
        free(novo);
        return false;
    }

    // Se o tamanho comprimido for maior que o original, não comprimimos
    if (tamanho_comprimido >= novo->tamanho_original) {
        printf("Tamanho comprimido maior que o original. Inserindo sem compressão.\n");
        free(buffer_original);
        free(buffer_comprimido);
        return inserir_membro(arq, path_membro);  // Chama função para inserir sem compressão
    }

    // Atualiza os campos do membro
    novo->tamanho_disco = tamanho_comprimido;
    novo->ordem = arq->num_membros;
    novo->comprimido = true;

    // Calcula o offset no arquivo
    fseek(arq->arquivo, 0, SEEK_END);
    novo->offset = ftell(arq->arquivo);

    // Grava o buffer comprimido no arquivo
    fwrite(buffer_comprimido, 1, tamanho_comprimido, arq->arquivo);

    // Adiciona o novo membro no array de membros
    arq->membros = realloc(arq->membros, sizeof(struct membro) * (arq->num_membros + 1));
    if (!arq->membros) {
        printf("Erro ao realocar memória para os membros.\n");
        free(buffer_original);
        free(buffer_comprimido);
        free(novo);
        return false;
    }

    arq->membros[arq->num_membros] = *novo;
    arq->num_membros++;

    // Libera a memória
    free(novo);
    free(buffer_original);
    free(buffer_comprimido);

    // Reabre o arquivo para atualizar a quantidade de membros e os dados
    rewind(arq->arquivo);
    fwrite(&arq->num_membros, sizeof(int), 1, arq->arquivo);
    fwrite(arq->membros, sizeof(struct membro), arq->num_membros, arq->arquivo);

    return true;
}

bool extrair_membro(struct archive *arq, int indice_membro)
{
    printf("MENBRO");
    struct membro *m = &arq->membros[indice_membro];
    printf("%s", m->nome);
    FILE *arquivo_saida = fopen(m->nome, "wb");

    if (!arquivo_saida) {
        printf("Erro ao criar o arquivo '%s'.\n", m->nome);
        return false;
    }

    fseek(arq->arquivo, 0, SEEK_END);  // Move para o final do arquivo
    long int arquivo_tamanho = ftell(arq->arquivo);  // Obtém o tamanho do arquivo
    if (m->offset < 0 || m->offset >= arquivo_tamanho) {
        printf("Erro: offset inválido para o membro '%s'. Offset: %ld, Tamanho do arquivo: %ld\n", m->nome, m->offset, arquivo_tamanho);
        fclose(arquivo_saida);
        return false;
    }
    // Posiciona o ponteiro do arquivo no offset correto do membro
    fseek(arq->arquivo, m->offset, SEEK_SET);

    unsigned char *buffer = malloc(m->tamanho_disco);
    if (!buffer) {
        printf("Erro ao alocar memória para o buffer.\n");
        fclose(arquivo_saida);
        return false;
    }

    // Lê os dados do membro comprimido ou não
    size_t bytes_lidos = fread(buffer, 1, m->tamanho_disco, arq->arquivo);
    if (bytes_lidos != (size_t)m->tamanho_disco) {
        printf("Erro ao ler os dados do arquivo para o membro '%s'.\n", m->nome);
        free(buffer);
        fclose(arquivo_saida);
        return false;
    }

    // Se o membro estiver comprimido, descomprimir
    if (m->comprimido) {
        printf("COm");
        unsigned char *buffer_descomprimido = malloc(m->tamanho_original);
        
        if (!buffer_descomprimido) {
            printf("Erro ao alocar memória para o buffer descomprimido.\n");
            free(buffer);
            fclose(arquivo_saida);
            return false;
        }

        LZ_Uncompress(buffer, buffer_descomprimido, m->tamanho_disco);
        if (!buffer_descomprimido) {
            printf("Erro ao descomprimir o arquivo '%s'.\n", m->nome);
            free(buffer);
            free(buffer_descomprimido);
            fclose(arquivo_saida);
            return false;
        }

        fwrite(buffer_descomprimido, 1, m->tamanho_original, arquivo_saida);
        free(buffer_descomprimido);
    } else {
        printf("no");
        fwrite(buffer, 1, m->tamanho_disco, arquivo_saida);
    }

    free(buffer);
    fclose(arquivo_saida);
    return true;
}


/*************************************************************************
*                           EXTERNAL FUNCTIONS                           *
*************************************************************************/

bool inserir_semCompressao(int argc, char *argv[])
{
    printf("VINAC MODE: [INSERT-NOCOMP]\n");

    struct archive *arq = create_archive();
    if(!arq){
        printf("Erro ao alocar arquivo");
        return false;
    }

    if(!abrir_archive(arq, argv[2]))
    {
        perror("VINAC [INSERT-NOCOM]: ERROR ao abrir o arquivo");
        return 1;
    }

    for(int i = 3; i < argc; i++){
        if(!inserir_membro(arq, argv[i]))
            fprintf(stderr, "[INSERT-NOCOM]: Erro ao inserir membro %s\n", argv[i]);
    }

    salvar_directorio(arq);
    fechar_arquive(arq);
    free(arq);

    printf("[INSERT-NOCOMP]: SUCCESS! \n");
    return true;
}

bool inserir_compressao(int argc, char *argv[])
{
    printf("VINAC MODE: [INSERT-COMP]\n");

    struct archive *arq = create_archive();
    if(!arq){
        printf("Erro ao alocar arquivo");
        return false;
    }

    if(!abrir_archive(arq, argv[2]))
    {
        perror("VINAC [INSERT-COM]: ERROR ao abrir o arquivo");
        return 1;
    }

    for(int i = 3; i < argc; i++){
        if(!inserir_membro_comprimido(arq, argv[i]))
            fprintf(stderr, "[INSERT-COM]: Erro ao inserir membro %s\n", argv[i]);
 
    }

    salvar_directorio(arq);
    fechar_arquive(arq);
    free(arq);

    printf("[INSERT-COM]: Sucess!\n");
    return true;
}

bool mover_membro(int argc, char *argv[])
{
    // Checando se o número correto de argumentos foi passado
    if (argc < 3) {
        fprintf(stderr, "VINAC [MOVE]: Parâmetros insuficientes.\n");
        return false;
    }

    struct archive *arq = carregar_arquivo(argv[2]);  

    char *membro_a_mover = argv[3];  
    char *membro_target = (argc > 4) ? argv[4] : NULL;  
    
    // Encontrar o índice do membro a ser movido
    int idx_a_mover = -1;
    for (int i = 0; i < arq->num_membros; i++) {
        if (strcmp(arq->membros[i].nome, membro_a_mover) == 0) {
            idx_a_mover = i;
            break;
        }
    }

    if (idx_a_mover == -1) {
        fprintf(stderr, "VINAC [MOVE]: Membro a ser movido não encontrado.\n");
        fechar_arquive(arq);
        return false;
    }
    
    // // Encontrar o índice do membro target (se houver)
    int idx_target = -1;
    if (membro_target != NULL) {
        for (int i = 0; i < arq->num_membros; i++) {
            if (strcmp(arq->membros[i].nome, membro_target) == 0) {
                idx_target = i;
                break;
            }
        }
    }

    // Se target for NULL, movemos para o início, se não, movemos após o target
    if (membro_target == NULL) {
        // Mover para o início, o membro a mover vai para a posição 0 e todos os outros membros são deslocados
        struct membro temp = arq->membros[idx_a_mover];
        
        for (int i = idx_a_mover; i > 0; i--) {
            arq->membros[i] = arq->membros[i - 1];
        }
        arq->membros[0] = temp;
    
    } else if (idx_target != -1) {
        // Mover após o target
        struct membro temp = arq->membros[idx_a_mover];

        // Se o membro a ser movido está antes do target, precisamos ajustar o loop de deslocamento
        if (idx_a_mover < idx_target) {
            for (int i = idx_a_mover; i < idx_target; i++) {
                arq->membros[i] = arq->membros[i + 1];
            }
            arq->membros[idx_target] = temp;
        } else { 
            // Caso contrário, o membro a ser movido está após o target, ajustamos para mover corretamente
            for (int i = idx_a_mover; i > idx_target + 1; i--) {
                arq->membros[i] = arq->membros[i - 1];
            }
            arq->membros[idx_target + 1] = temp;
        }
    } else {
        fprintf(stderr, "VINAC [MOVE]: Target não encontrado.\n");
        fechar_arquive(arq);
        return false;
    }

    // Atualizar os offsets dos membros (importantíssimo)
    long offset_atual = sizeof(int) + sizeof(struct membro) * arq->num_membros;
    for (int i = 0; i < arq->num_membros; i++) {
        arq->membros[i].offset = offset_atual;
        offset_atual += arq->membros[i].tamanho_disco;
    }

     // Salvar o arquivo com a nova ordem
    if (!salvar_directorio(arq)) {
        fprintf(stderr, "VINAC [MOVE]: Erro ao salvar o diretório.\n");
        fechar_arquive(arq);
        return false;
    }

    printf("[MOVE]: Membro %s movido com sucesso.\n", membro_a_mover);

    fechar_arquive(arq);
    free(arq);
    return true;
}

bool extrair_membros(int argc, char *argv[])
{
    printf("%d", argc);

    struct archive *arq = carregar_arquivo(argv[2]);
    if(!arq){
        printf("Erro ao alocar arquivo");
        return false;
    }

    printf("%d", arq->num_membros);

    if (argc == 3) {
         // Se nenhum membro for especificado, extrai todos os membros
         for (int i = 0; i < arq->num_membros; i++) {
            if (!extrair_membro(arq, i)) {
                printf("Erro ao extrair o membro %s\n", arq->membros[i].nome);
                return false;
            }
         }
    } //else {
    //     // Caso contrário, extrai os membros especificados
    //     for (int i = 3; i < argc; i++) {
    //         bool encontrado = false;
    //         for (int j = 0; j < arq->num_membros; j++) {
    //             if (strcmp(argv[i], arq->membros[j].nome) == 0) {
    //                 // Encontrou o membro correspondente
    //                 if (!extrair_membro(arq, j)) {
    //                     printf("Erro ao extrair o membro %s\n", arq->membros[j].nome);
    //                     return false;
    //                 }
    //                 encontrado = true;
    //                 break;
    //             }
    //         }
    //         if (!encontrado) {
    //             printf("Membro '%s' não encontrado no archive.\n", argv[i]);
    //             return false;
    //         }
    //     }
    // }

    return true;
}

bool remover(int argc, char *argv[])
{
    // Checando se o número correto de argumentos foi passado
    if (argc < 3) {
        fprintf(stderr, "VINAC [REMOVE]: Parâmetros insuficientes.\n");
        return false;
    }

    struct archive *arq = carregar_arquivo(argv[2]);

    if (argc == 3) {
        // Nenhum membro especificado, remover todo o arquivo
        printf("[REMOVE]: Removendo o arquivo inteiro: %s\n", argv[2]);
        
        // Salvar o arquivo vazio (sem membros)
        arq->num_membros = 0;
        if (!salvar_directorio(arq)) {
            fprintf(stderr, "VINAC [REMOVE]: Erro ao salvar o diretório.\n");
            fechar_arquive(arq);
            return false;
        }

        printf("[REMOVE]: Arquivo %s removido com sucesso.\n", argv[2]);
    } else {
        // Membros especificados, removê-los
        for (int i = 3; i < argc; i++) {
            char *membro_a_remover = argv[i];

            int idx_a_remover = -1;
            for (int j = 0; j < arq->num_membros; j++) {
                if (strcmp(arq->membros[j].nome, membro_a_remover) == 0) {
                    idx_a_remover = j;
                    break;
                }
            }

            if (idx_a_remover == -1) {
                fprintf(stderr, "VINAC [REMOVE]: Membro %s não encontrado.\n", membro_a_remover);
                continue; // Continua com os próximos membros
            }

            // Desloca os membros para remover o membro indicado
            for (int j = idx_a_remover; j < arq->num_membros - 1; j++) {
                arq->membros[j] = arq->membros[j + 1];
            }

            // Decrementa o número de membros
            arq->num_membros--;

            printf("[REMOVE]: Membro %s removido com sucesso.\n", membro_a_remover);
        }

        // Salvar o arquivo após remoção dos membros
        if (!salvar_directorio(arq)) {
            fprintf(stderr, "VINAC [REMOVE]: Erro ao salvar o diretório.\n");
            fechar_arquive(arq);
            return false;
        }
    }

    fechar_arquive(arq);
    free(arq);
    return true;
}

bool listar(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <arquivo.vinac>\n", argv[0]);
        return false;
    }

    struct archive *arq = carregar_arquivo(argv[2]);
    if (!arq) {
        fprintf(stderr, "Erro ao carregar o arquivo %s\n", argv[1]);
        return false;
    }

    printf("Lista de membros do arquivo '%s':\n", argv[1]);
    printf("------------------------------------------------------------\n");
    printf("Ordem | Nome                 | UID   | Orig  | Disco | Data de modificação\n");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < arq->num_membros; ++i) {
        struct membro m = arq->membros[i];

        // Converte a data para string
        char data_str[64];
        struct tm *tm_info = localtime(&m.data_modificacao);
        strftime(data_str, sizeof(data_str), "%Y-%m-%d %H:%M:%S", tm_info);

        printf("%5d | %-20s | %5d | %5zu | %5zu | %s\n",
            i + 1,
            m.nome,
            m.uid,
            m.tamanho_original,
            m.tamanho_disco,
            data_str);
    }

    fechar_arquive(arq);
    return true;
}