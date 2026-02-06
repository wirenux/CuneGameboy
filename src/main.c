#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "../includes/cpu.h"
#include "../includes/main.h"

char getch_nonblocking(void) {
	struct termios oldt, newt;
	char ch = '\0';
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	newt.c_cc[VMIN] = 0;
	newt.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	if (read(STDIN_FILENO, &ch, 1) > 0) {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		return ch;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return '\0';
}

int load_rom(GameBoy *gb, const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "\033[1;31mError: Could not open ROM %s\033[0m\n", filename);
		return 0;
	}
	fread(gb->memory, 1, 0x8000, file);
	fclose(file);
	return 1;
}

int main(int argc, char *argv[]) {
	CPU cpu;
	int run_mode = 0; // 0 = step mode, 1 = run mode
	GameBoy *gb = malloc(sizeof(GameBoy));
	if (!gb) return 1;
	cpu_init(&gb->cpu);
	if (!load_rom(gb, argv[1])) {
		free(gb);
		return 1;
	}
	printf("\033[0;32mROM Loaded.\033[0m Starting emulation at PC: 0x%04X\n", gb->cpu.PC);
	printf("\033[0;35m[Controls: SPACE = Run/Pause toggle, ENTER = Step one opcode]\033[0m\n");
	sleep(1);

	while (1) {
		cpu_print_state(gb);
		if (run_mode) {
			printf("\033[1;32m[RUNNING MODE]\033[0m Press SPACE to pause\n");
		} else {
			printf("\033[1;33m[STEP MODE]\033[0m Press ENTER to step, SPACE to run\n");
		}

		if (run_mode) {
			// Non-blocking input check
			char input = getch_nonblocking();
			if (input == ' ') {
				run_mode = 0;
				printf("\033[0;35m[Paused]\033[0m\n");
				continue;
			}
			cpu_step(gb);
			usleep(100); // time b/w each opcode
		} else {
			// Blocking input in step mode
			int c = getchar();
			if (c == ' ') {
				run_mode = 1;
				continue;
			} else if (c == '\n') {
				cpu_step(gb);
			}
		}
	}
	free(gb);
	return 0;
}