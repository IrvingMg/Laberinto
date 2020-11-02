# Compilar y ejecutar
.PHONY: all
all: build  run

# Compilar
.PHONY: build
build:
	gcc -o laberinto.out laberinto.c

# Ejecutar
.PHONY: run
run:
	chmod +x laberinto.out
	./laberinto.out

# Eliminar ejecutables
.PHONY: clean
clean:
	rm *.out
