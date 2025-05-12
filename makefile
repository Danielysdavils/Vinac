# MAKEFILE VINAC

#  Autora: Danielys Davila
#  Versão: v0.0.1
#  Ultima revisão: 11/05/2025

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -std=c99 \
          -I. -Ientidades -Iservices -Ilibrary/lista -Ilibrary/lz
LDLIBS  = -lm
MAIN    = login/vina
ENTREGA = vina

# lista de arquivos de cabeçalho
HDR = library/lista/lista.h library/lz/lz.h entidades/actions.h entidades/archive.h entidades/membro.h

# lista de arquivos-objeto
OBJ = library/lista/lista.o library/lz/lz.o \
      services/archive.o services/actions.o services/membro.o

# diretório de saída
DIR = login

# criar diretório, se não existir
$(DIR):
	mkdir -p $(DIR)

# compilar tudo (requisito: all)
all: $(DIR) $(MAIN)

# construir o executável
$(MAIN): $(OBJ) vinac.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

vinac.o: vinac.c $(HDR)

library/lista/lista.o: library/lista/lista.c library/lista/lista.h
	$(CC) $(CFLAGS) -c -o $@ $<

library/lz/lz.o: library/lz/lz.c library/lz/lz.h
	$(CC) $(CFLAGS) -c -o $@ $<

services/actions.o: services/actions.c entidades/actions.h
	$(CC) $(CFLAGS) -c -o $@ $<

services/archive.o: services/archive.c entidades/archive.h
	$(CC) $(CFLAGS) -c -o $@ $<

services/membro.o: services/membro.c entidades/membro.h
	$(CC) $(CFLAGS) -c -o $@ $<

# executar
run: $(MAIN)
	./$(MAIN)

# testar no Valgrind
valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)

# limpar arquivos temporários (requisito: clean)
clean:
	rm -f *.o */*.o vinac.o $(MAIN)
	rm -rf $(DIR)

# gerar arquivo TGZ para entrega
tgz: clean
	-mkdir -p /tmp/$(USER)/$(ENTREGA)
	chmod 0700 /tmp/$(USER)/$(ENTREGA)
	cp vinac.c *.c *.h makefile entidades/*.h services/*.c library/lista/*.c library/lz/*.c library/lista/*.h library/lz/*.h /tmp/$(USER)/$(ENTREGA)