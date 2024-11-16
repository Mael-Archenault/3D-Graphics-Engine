all:
	gcc main.c drawing_functions.c -o  main.exe -Isrc/include -Lsrc/lib -lSDL2 -lSDL2main