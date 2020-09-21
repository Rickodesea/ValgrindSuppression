CC = gcc 
CFLAGS = -g -W 
INCS = -Iglad 
LIBS = -lGL -ldl -lSDL2
OBJ = glad.o SDLDemo.o 
APP = SDLDemo.app 

supp-run: build
	valgrind --gen-suppressions=all --suppressions=linux_sdl_gl.sup --leak-check=full --show-leak-kinds=all ./$(APP)

nosupp-run: build
	valgrind --leak-check=full --show-leak-kinds=all ./$(APP)

glad.o: glad/glad.c
	$(CC) $(INCS) $(CFLAGS) -c glad/glad.c -o glad.o

SDLDemo.o: SDLDemo.c
	$(CC) $(INCS) $(CFLAGS) -c SDLDemo.c -o SDLDemo.o

$(APP): $(OBJ)
	$(CC) $(OBJ) $(INCS) $(CFLAGS) $(LIBS) -o $(APP)

build: $(APP)

noval-run: build
	./$(APP)

clean:
	rm -f *.o
	rm -f *.app

