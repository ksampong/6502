#include "cpu.h"


int main() {
    Memory mem;
    CPU cpu;
    cpu.Reset(mem);
    mem.Initialise();
    mem[0XFFFC] = CPU::INS_LDA_INX;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;
    cpu.Execute(6, mem);
    return 0;
}