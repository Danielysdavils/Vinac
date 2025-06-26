VINAC - Arquivador com suporte a compressão escrito na linguagem C

Autora: Danielys Davila


Arquivos:
    # unicamente cabeçalhos (structs) modulares
    /entidades
        # declaração de funções "comandos" disponíveis a realizar no vinac 
        - actions.h

        # declaração de estrutura archive
        - archive.h
  
        # declaração de estrutura membro
        - membro.h
  
    # bibliotecas externas
    /library
        /lz
            - lz.h
            - lz.c
    
    # serviços/funções referentes aos cabeçalhos 
    /services
        # funções referentes às declarações do actions.h
        - actions.c
  
        # funções referentes às declarações do actions.c
        - achive.c

        # funções referentes às declarações do membro.c
        - membro.c
  
    # arquivo main
    - vinac.c
    - readme.md
    - makefile

Implementações: 
    - Estrutura modular pensando em clases (structs .h) e serviços(.c) visando melhor legivilidade e organização de código.

    - Inicialmente foi considerado implementar uma estrutura de lista encadeada para organização dos membros nos archives mas a ideia foi descartada por falta de tempo.

Bugs:
    - No momento, a implementação da inserção, seja de arquivos com ou sem compressão, está limitada a uma unica tentaiva por archive, e caso deseje inserir mais elementos depois da criação esse novo elemento irá reemplazar os demais pre-existentes. É esperado que na próxima atualização desta versão esse bug seja concertado.
  
    - Ao remover, quando não é passado nenhum parametro espera-se remover o arquivo archive da memória, mas, no momento, só deleta todos os membros sem remover de fato o arquivo.  


