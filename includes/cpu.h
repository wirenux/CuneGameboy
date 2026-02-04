#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {
    struct {
        union {
            struct { uint8_t F; uint8_t A; };
            uint16_t AF;
        };
    };
    struct {
        union {
            struct { uint8_t C; uint8_t B; };
            uint16_t BC;
        };
    };
    struct {
        union {
            struct { uint8_t E; uint8_t D; };
            uint16_t DE;
        };
    };
    struct {
        union {
            struct { uint8_t L; uint8_t H; };
            uint16_t HL;
        };
    };

    uint16_t SP; // Stack Pointer
    uint16_t PC; // Program Counter

    uint32_t CYCLES; // Keep track of timing
} CPU;

#define FLAG_Z (1 << 7) // Zero
#define FLAG_N (1 << 6) // Subtract
#define FLAG_H (1 << 5) // Half-Carry
#define FLAG_C (1 << 4) // Carry

void cpu_init(CPU *cpu);

#endif