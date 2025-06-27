CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude -I/opt/homebrew/opt/sdl3/include
LDFLAGS = -L/opt/homebrew/opt/sdl3/lib
LIBS = -lSDL3 -framework OpenGL -framework OpenCL -ldl

SRC = src/*.c 

OUT = prog

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS) $(LIBS)

run: build
	DYLD_LIBRARY_PATH=/opt/homebrew/opt/sdl3/lib ./$(OUT)

clean:
	rm -f $(OUT)
