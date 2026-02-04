all:
	gcc -Iincludes src/*.c -o build/emu

run: all
	./build/emu assets/roms/Tetris.gb

clean:
	rm -rf build/emu