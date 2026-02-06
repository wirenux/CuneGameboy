#include "../includes/main.h"
#include <stdio.h>

void execute_instruction(GameBoy *gb, uint8_t opcode) {
    switch (opcode) {
        case 0x00: { // NOP
            gb->cpu.PC += 1;
            gb->cpu.CYCLES += 4;
            break;
        }
        case 0x05: { // DEC B
            int h_flag = (gb->cpu.B & 0x0F) == 0x00;

            gb->cpu.B --;

            gb->cpu.F &= FLAG_C; // Keep the old Carry flag, clear others
            if (gb->cpu.B == 0) gb->cpu.F |= FLAG_Z;
            gb->cpu.F |= FLAG_N;
            if (h_flag) gb->cpu.F |= FLAG_H;

            gb->cpu.PC += 1;
            gb->cpu.CYCLES += 4;
            break;
        }
        case 0x06: { // LD B, d8 (Load 8-bit immediate into C)
            // Fetch the byte immediately following the opcode
            gb->cpu.B = gb->memory[gb->cpu.PC + 1];

            gb->cpu.PC += 2;
            gb->cpu.CYCLES += 8;
            break;
        }
        case 0x0D: { // DEC C
            int h_flag = (gb->cpu.C & 0x0F) == 0x00;

            gb->cpu.C--;

            gb->cpu.F &= FLAG_C;
            if (gb->cpu.C == 0) gb->cpu.F |= FLAG_Z;
            gb->cpu.F |= FLAG_N;
            if (h_flag) gb->cpu.F |= FLAG_H;

            gb->cpu.PC += 1;
            gb->cpu.CYCLES += 4;
            break;
        }
        case 0x0E: { // LD C, d8 (Load 8-bit immediate into C)
            // Fetch the byte immediately following the opcode
            gb->cpu.C = gb->memory[gb->cpu.PC + 1];

            gb->cpu.PC += 2;
            gb->cpu.CYCLES += 8;
            break;
        }
        case 0x20: { // JR NZ, r8

            int8_t offset = (int8_t)gb->memory[gb->cpu.PC + 1];

            gb->cpu.PC += 2;

            // Z clear?
            if (!(gb->cpu.F & FLAG_Z)) {
                // Jump
                gb->cpu.PC += offset;
                gb->cpu.CYCLES += 12; // 3 cycles
            } else {
                // Not jump
                gb->cpu.CYCLES += 8;  // 2 cycles
            }
            break;
        }
        case 0x21: { // LD HL, d16 (Load 16-bit immediate into HL)
            uint8_t low = gb->memory[gb->cpu.PC + 1];
            uint16_t high = gb->memory[gb->cpu.PC + 2];

            // Combine them into the 16-bit HL register
            gb->cpu.HL = (high << 8) | low;

            gb->cpu.PC += 3;
            gb->cpu.CYCLES += 12;
            break;
        }
        case 0x32: { // LD (HL-), A
            uint16_t address = gb->cpu.HL;

            gb->memory[address] = gb->cpu.A;

            gb->cpu.HL--;

            gb->cpu.PC += 1;
            gb->cpu.CYCLES += 8;
            break;
        }
        case 0x3E: { // LD A, d8 (Load 8-bit immediate into C)
            // Fetch the byte immediately following the opcode
            gb->cpu.A = gb->memory[gb->cpu.PC + 1];

            gb->cpu.PC += 2;
            gb->cpu.CYCLES += 8;
            break;
        }
        case 0xAF:
            gb->cpu.A ^= gb->cpu.A;
            gb->cpu.F = FLAG_Z;
            gb->cpu.PC += 1;
            gb->cpu.CYCLES += 4;
            break;
        case 0xC3: { // JP nn (Jump to 16-bit address)
            // Little Endian
            uint16_t low = gb->memory[gb->cpu.PC + 1];
            uint16_t high = gb->memory[gb->cpu.PC + 2];
            uint16_t target = (high << 8) | low;
            gb->cpu.PC = target;
            gb->cpu.CYCLES += 16;
            break;
        }
        case 0xE0: { // LD (a8), A
            uint8_t offset = gb->memory[gb->cpu.PC + 1];

            uint16_t target_addr = 0xFF00 + offset;

            gb->memory[target_addr] = gb->cpu.A;

            gb->cpu.PC += 2;
            gb->cpu.CYCLES += 12;
            break;
        }
        case 0xF3: { // DI (Disable Interrupts)
            gb->cpu.ime = 0;

            gb->cpu.PC += 1;
            gb->cpu.CYCLES += 4;
            break;
        }
        default:
            break;
    }
}