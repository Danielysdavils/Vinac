# makefile The Boys
# Carlos Maziero - DINF/UFPR, 2024/2

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -std=c99 \
          -Ientidades -Iservices -Ilibrary/lista -Ilibrary/lz
LDLIBS  = -lm
MAIN    = vinac
ENTREGA = $(MAIN)

# lista de arquivos de cabeçalho
HDR = library/lista/lista.h library/lz/lz.h entidades/actions.h entidades/archive.h entidades/membro.h

# lista de arquivos-objeto
OBJ = library/lista/lista.o library/lz/lz.o \
      services/archive.o services/actions.o services/membro.o

# construir o executável
$(MAIN): $(OBJ) $(MAIN).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# arquivos objeto principais
$(MAIN).o: vinac.c $(HDR)

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

# gerar arquivo TGZ para entrega
tgz: clean
	-mkdir -p /tmp/$(USER)/$(ENTREGA)
	chmod 0700 /tmp/$(USER)/$(ENTREGA)
	cp vinac.c *.c *.h makefile entidades/*.h services/*.c library/lista/*.c library/lz/*.c library/lista/*.h library/lz/*.h /tmp/$(USER)/$(ENTRE
