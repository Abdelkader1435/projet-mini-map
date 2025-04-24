prog: main.o minimap.o 
	gcc main.o minimap.o -o prog -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

main.o: main.c
	gcc -c main.c -g

minimap.o: minimap.c
	gcc -c minimap.c -g
