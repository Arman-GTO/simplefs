all: fs

fs: main.c fs.c fs.h
	gcc -o fs main.c fs.c
