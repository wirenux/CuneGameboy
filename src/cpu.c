#include "main.h"
#include <stdio.h>
#include <string.h>

void cpu_init(CPU *cpu) {
    memset(cpu, 0, sizeof(CPU));

    cpu->AF = 0x01B0;
    cpu->BC = 0x0013;
    cpu->DE = 0x00D8;
    cpu->HL = 0x014D;
    cpu->SP = 0xFFFE;
    cpu->PC = 0x0100;
    cpu->CYCLES = 0;
}

void cpu_step(GameBoy *gb) {
    uint8_t opcode = gb->memory[gb->cpu.PC];

    // execute_instruction(gb, opcode);
}

void cpu_print_state(GameBoy *gb) {
    CPU *c = &gb->cpu;
    uint8_t op = gb->memory[c->PC];

    // cursor top-left
    printf("\033[2J");
    printf("\033[H");

    printf("\033[1;36m--- GAMEBOY CPU STATE ---\033[0m\n");
    printf("Registers:          Flags: \n");
    printf("AF: 0x%04X          Z: %d\n", c->AF, (c->F & FLAG_Z) ? 1 : 0);
    printf("BC: 0x%04X          N: %d\n", c->BC, (c->F & FLAG_N) ? 1 : 0);
    printf("DE: 0x%04X          H: %d\n", c->DE, (c->F & FLAG_H) ? 1 : 0);
    printf("HL: 0x%04X          C: %d\n", c->HL, (c->F & FLAG_C) ? 1 : 0);
    printf("--------------------------\n");
    printf("PC: 0x%04X    SP: 0x%04X\n", c->PC, c->SP);
    printf("Opcode: \033[1;33m0x%02X\033[0m\n", op);
    printf("--------------------------\n");
}