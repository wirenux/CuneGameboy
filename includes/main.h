#include "cpu.h"

typedef struct {
    CPU cpu;
    uint8_t memory[0x10000]; // 64KB
} GameBoy;