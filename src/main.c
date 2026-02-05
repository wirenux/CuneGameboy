#include <stdio.h>
#include <stdlib.h>
#include "../includes/cpu.h"
#include "../includes/main.h"

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

    GameBoy *gb = malloc(sizeof(GameBoy));
    if (!gb) return 1;

    cpu_init(&gb->cpu);

    if (!load_rom(gb, argv[1])) {
        free(gb);
        return 1;
    }

    printf("\033[0;32mROM Loaded.\033[0m Starting emulation at PC: 0x%04X\n", gb->cpu.PC);

    while (1) {
        cpu_print_state(gb);
        getchar();
    }

    free(gb);
    return 0;
}