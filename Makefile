all: main.o pesquisa.o registro.o util.o index.o arvore.o arvoreb.o arvorebstar.o
	@gcc src/main.o src/pesquisa/pesquisa.o src/registro/registro.o src/util/util.o src/index/index.o src/arvore/arvore.o src/arvoreb/arvoreb.o src/arvorebstar/arvorebstar.o -o pesquisa_exe
	@rm src/main.o src/pesquisa/pesquisa.o src/registro/registro.o src/util/util.o src/index/index.o src/arvore/arvore.o src/arvoreb/arvoreb.o src/arvorebstar/arvorebstar.o

main.o: src/main.c
	@gcc -c src/main.c -Wall -Isrc/index -Isrc/pesquisa -Isrc/arvore -Isrc/arvoreb -Isrc/arvorebstar -Isrc/util -o src/main.o

pesquisa.o: src/pesquisa/pesquisa.c src/pesquisa/pesquisa.h
	@gcc -c src/pesquisa/pesquisa.c -Wall -Isrc/index -Isrc/pesquisa -Isrc/arvore -Isrc/arvoreb -Isrc/arvorebstar -o src/pesquisa/pesquisa.o

registro.o: src/registro/registro.c src/registro/registro.h
	@gcc -c src/registro/registro.c -Wall -o src/registro/registro.o

util.o: src/util/util.c src/util/util.h
	@gcc -c src/util/util.c -Wall -o src/util/util.o

index.o: src/index/index.c src/index/index.h
	@gcc -c src/index/index.c -Wall -o src/index/index.o

arvore.o: src/arvore/arvore.c src/arvore/arvore.h
	@gcc -c src/arvore/arvore.c -Wall -o src/arvore/arvore.o

arvoreb.o: src/arvoreb/arvoreb.c src/arvoreb/arvoreb.h
	@gcc -c src/arvoreb/arvoreb.c -Wall -o src/arvoreb/arvoreb.o

arvorebstar.o: src/arvorebstar/arvorebstar.c src/arvorebstar/arvorebstar.h
	@gcc -c src/arvorebstar/arvorebstar.c -Wall -o src/arvorebstar/arvorebstar.o

run:
	@./pesquisa_exe $(ARGS)

# Exemplo de uso: make run ARGS="1 1000 1 12345"
