robot.exe : fct_createmap.o fct_robotext.o fct_robotfeu.o main.o fct_sdl.o 
	gcc fct_createmap.o fct_robotext.o fct_robotfeu.o main.o fct_sdl.o -o robot.exe -lSDL -lSDL_ttf -lSDL_mixer -Wall -lSDL_image -Wconversion
fct_createmap.o : fct_createmap.c main.h
	gcc -c fct_createmap.c 
fct_robotext.o : fct_robotext.c main.h
	gcc -c fct_robotext.c
fct_robotfeu.o : fct_robotfeu.c main.h
	gcc -c fct_robotfeu.c
fct_sdl.o : fct_sdl.c main.h fct_sdl.h
	gcc -c fct_sdl.c
main.o : main.c main.h fct_sdl.h
	gcc -c main.c

ifeq ($(OS),Windows_NT)
clean :
	del *.o *.gch robot.exe
	#Clean
else
clean :
	rm *.o *.gch robot.exe
	#Clean
endif

