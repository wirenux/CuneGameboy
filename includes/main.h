#ifndef MAIN_H
#define MAIN_H

#include "cpu.h"

typedef struct {
    CPU cpu;
    uint8_t memory[0x10000]; // 64KB
} GameBoy;

void cpu_step(GameBoy *gb);
void cpu_print_state(GameBoy *gb);

void execute_instruction(GameBoy *gb, uint8_t opcode);

#endif