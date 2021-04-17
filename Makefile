DIR_HEADER_LIB_PATH=headers/
COMP_FILES=src/*.c
OBJECT_FILES=src/*.o
LIBSTATIC=-lSDL -lm -lSDL_mixer -lSDL_image -lSDL_ttf -lSDL_gfx

all:
	gcc -I${DIR_HEADER_LIB_PATH} ${COMP_FILES} -c 
	mv *.o src/
clean:
	rm src/*.o
