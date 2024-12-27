#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

struct Memory {
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise();
    Byte operator[](u32 Address) const;
    Byte& operator[](u32 Address);
    void writeWord(Word Value, u32 Address, u32& Cycles);
};

struct CPU {
    Word PC; // Program Counter
    Byte SP; // Stack Pointer
    Byte A;  // Accumulator
    Byte X;  // Index Register
    Byte Y;  // Index Register
    int Cycles; // Number of cycles


    Byte C : 1; // Carry flag
    Byte Z : 1; // Zero flag
    Byte I : 1; // Interrupt flag
    Byte D : 1; // Decimal mode flag
    Byte B : 1; // Break flag
    Byte V : 1; // Overflow flag
    Byte N : 1; // Negative flag

    void Reset(Memory& memory);
    Byte FetchByte(u32& Cycles, Memory& memory);
    Word FetchWord(u32& Cycles, Memory& memory);
    Byte ReadByte(u32& Cycles, Byte Address, Memory& memory);

    // Opcodes
    //load
    static constexpr Byte INS_LDA_IM = 0xA9;
    static constexpr Byte INS_LDA_ZP = 0xA5;
    static constexpr Byte INS_LDA_ZPX = 0xB5;
    static constexpr Byte INS_LDA_AB = 0xAD;
    static constexpr Byte INS_LDA_ABX = 0xBD;
    static constexpr Byte INS_LDA_ABY = 0xB9;
    static constexpr Byte INS_LDA_INX = 0xA1;
    static constexpr Byte INS_LDA_INY = 0xB1;
    static constexpr Byte INS_LDX_IM = 0xA2;
    static constexpr Byte INS_LDX_ZP = 0xA6;
    static constexpr Byte INS_LDX_ZPY = 0xB6;
    static constexpr Byte INS_LDX_AB = 0xAE;
    static constexpr Byte INS_LDX_ABY = 0xBE;
    static constexpr Byte INS_LDY_IM = 0xA0;
    static constexpr Byte INS_LDY_ZP = 0xA4;
    static constexpr Byte INS_LDY_ZPX = 0xB4;
    static constexpr Byte INS_LDY_AB = 0xAC;
    static constexpr Byte INS_LDY_ABX = 0xBC;
    //store
    static constexpr Byte INS_STA_AB = 0x8D;
    static constexpr Byte INS_STA_ABX = 0x9D;
    static constexpr Byte INS_STA_ABY = 0x99;
    static constexpr Byte INS_STA_INX = 0x81;
    static constexpr Byte INS_STA_INY = 0x91;
    static constexpr Byte INS_STA_ZP = 0x85;
    static constexpr Byte INS_STA_ZPX = 0x95;
    static constexpr Byte INS_STX_ZP = 0x86;
    static constexpr Byte INS_STX_ZPY = 0x96;
    static constexpr Byte INS_STX_AB = 0x8E;
    static constexpr Byte INS_STY_ZP = 0x84;
    static constexpr Byte INS_STY_ZPX = 0x94;
    static constexpr Byte INS_STY_AB = 0x8C;
    //transfer
    static constexpr Byte INS_TAX = 0xAA;
    static constexpr Byte INS_TAY = 0xA8;
    static constexpr Byte INS_TXA = 0x8A;
    static constexpr Byte INS_TYA = 0x98;
    //jump
    static constexpr Byte INS_JMP_IND = 0x6C;
    static constexpr Byte INS_JMP_AB = 0x4C;
    static constexpr Byte INS_JSR = 0x20;
    static constexpr Byte INS_RTS = 0x60;
    //branch
    static constexpr Byte INS_BCC = 0x90;
    static constexpr Byte INS_BCS = 0xB0;
    static constexpr Byte INS_BEQ = 0xF0;
    static constexpr Byte INS_BMI = 0x30;
    static constexpr Byte INS_BNE = 0xD0;
    static constexpr Byte INS_BPL = 0x10;
    static constexpr Byte INS_BVC = 0x50;
    static constexpr Byte INS_BVS = 0x70;
    //stack
    static constexpr Byte INS_TSX = 0xBA;
    static constexpr Byte INS_TXS = 0x9A;
    static constexpr Byte INS_PHA = 0x48;
    static constexpr Byte INS_PHP = 0x08;
    static constexpr Byte INS_PLA = 0x68;
    static constexpr Byte INS_PLP = 0x28;
    //flag
    static constexpr Byte INS_CLC = 0x18;
    static constexpr Byte INS_CLD = 0xD8;
    static constexpr Byte INS_CLI = 0x58;
    static constexpr Byte INS_CLV = 0xB8;
    static constexpr Byte INS_SEC = 0x38;

    //arithmetic
    static constexpr Byte INS_ADC_IM = 0x69;
    static constexpr Byte INS_ADC_ZP = 0x65;
    static constexpr Byte INS_ADC_ZPX = 0x75;
    static constexpr Byte INS_ADC_AB = 0x6D;
    static constexpr Byte INS_ADC_ABX = 0x7D;
    static constexpr Byte INS_ADC_ABY = 0x79;
    static constexpr Byte INS_ADC_INX = 0x61;
    static constexpr Byte INS_ADC_INY = 0x71;
    static constexpr Byte INS_SBC_IM = 0xE9;
    static constexpr Byte INS_SBC_ZP = 0xE5;
    static constexpr Byte INS_SBC_ZPX = 0xF5;
    static constexpr Byte INS_SBC_AB = 0xED;
    static constexpr Byte INS_SBC_ABX = 0xFD;
    static constexpr Byte INS_SBC_ABY = 0xF9;
    static constexpr Byte INS_SBC_INX = 0xE1;
    static constexpr Byte INS_SBC_INY = 0xF1;
    //compare
    static constexpr Byte INS_CMP_IM = 0xC9;
    static constexpr Byte INS_CMP_ZP = 0xC5;
    static constexpr Byte INS_CMP_ZPX = 0xD5;
    static constexpr Byte INS_CMP_AB = 0xCD;
    static constexpr Byte INS_CMP_ABX = 0xDD;
    static constexpr Byte INS_CMP_ABY = 0xD9;
    static constexpr Byte INS_CMP_INX = 0xC1;
    static constexpr Byte INS_CMP_INY = 0xD1;
    static constexpr Byte INS_CPX_IM = 0xE0;
    static constexpr Byte INS_CPX_ZP = 0xE4;
    static constexpr Byte INS_CPX_AB = 0xEC;
    static constexpr Byte INS_CPY_IM = 0xC0;
    static constexpr Byte INS_CPY_ZP = 0xC4;
    static constexpr Byte INS_CPY_AB = 0xCC;
    //inc dec
    static constexpr Byte INS_DEC_ZP = 0xC6;
    static constexpr Byte INS_DEC_ZPX = 0xD6;
    static constexpr Byte INS_DEC_AB = 0xCE;
    static constexpr Byte INS_DEC_ABX = 0xDE;
    static constexpr Byte INS_DEX = 0xCA;
    static constexpr Byte INS_DEY = 0x88;
    static constexpr Byte INS_INC_ZP = 0xE6;
    static constexpr Byte INS_INC_ZPX = 0xF6;
    static constexpr Byte INS_INC_AB = 0xEE;
    static constexpr Byte INS_INC_ABX = 0xFE;
    static constexpr Byte INS_INX = 0xE8;
    static constexpr Byte INS_INY = 0xC8;
    //shift
    static constexpr Byte INS_ASL_IM = 0x0A;
    static constexpr Byte INS_ASL_ZP = 0x06;
    static constexpr Byte INS_ASL_ZPX = 0x16;
    static constexpr Byte INS_ASL_AB = 0x0E;
    static constexpr Byte INS_ASL_ABX = 0x1E;
    static constexpr Byte INS_LSR_IM = 0x4A;
    static constexpr Byte INS_LSR_ZP = 0x46;
    static constexpr Byte INS_LSR_ZPX = 0x56;
    static constexpr Byte INS_LSR_AB = 0x4E;
    static constexpr Byte INS_LSR_ABX = 0x5E;
    static constexpr Byte INS_ROL_IM = 0x2A;
    static constexpr Byte INS_ROL_ZP = 0x26;
    static constexpr Byte INS_ROL_ZPX = 0x36;
    static constexpr Byte INS_ROL_AB = 0x2E;
    static constexpr Byte INS_ROL_ABX = 0x3E;
    static constexpr Byte INS_ROR_IM = 0x6A;
    static constexpr Byte INS_ROR_ZP = 0x66;
    static constexpr Byte INS_ROR_ZPX = 0x76;
    static constexpr Byte INS_ROR_AB = 0x6E;
    static constexpr Byte INS_ROR_ABX = 0x7E;
    static
    //bitwise
    static constexpr Byte INS_AND_IM = 0x29;
    static constexpr Byte INS_AND_ZP = 0x25;
    static constexpr Byte INS_AND_ZPX = 0x35;
    static constexpr Byte INS_AND_AB = 0x2D;
    static constexpr Byte INS_AND_ABX = 0x3D;
    static constexpr Byte INS_AND_ABY = 0x39;
    static constexpr Byte INS_AND_INX = 0x21;
    static constexpr Byte INS_AND_INY = 0x31;
    static constexpr Byte INS_ORA_IM = 0x09;
    static constexpr Byte INS_ORA_ZP = 0x05;
    static constexpr Byte INS_ORA_ZPX = 0x15;
    static constexpr Byte INS_ORA_AB = 0x0D;
    static constexpr Byte INS_ORA_ABX = 0x1D;
    static constexpr Byte INS_ORA_ABY = 0x19;
    static constexpr Byte INS_ORA_INX = 0x01;
    static constexpr Byte INS_ORA_INY = 0x11;
    static constexpr Byte INS_EOR_IM = 0x49;
    static constexpr Byte INS_EOR_ZP = 0x45;
    static constexpr Byte INS_EOR_ZPX = 0x55;
    static constexpr Byte INS_EOR_AB = 0x4D;
    static constexpr Byte INS_EOR_ABX = 0x5D;
    static constexpr Byte INS_EOR_ABY = 0x59;
    static constexpr Byte INS_EOR_INX = 0x41;
    static constexpr Byte INS_EOR_INY = 0x51;

    static constexpr Byte INS_SED = 0xF8;

    static constexpr Byte INS_SEI = 0x78;

    // Status flag operations
    void LDRSetStatus();
    void CMPSetStatus(Byte M, Byte A);
    void TRSetStatus(Byte M);
    void ADCSetStatus(Byte M);
    void SBCSetStatus(Byte M);

    // Addressing modes
    Byte ZeroPageadress(u32 Cycles, Memory& memory);
    Byte ZeroPageadressX(u32 Cycles, Memory& memory);
    Byte ZeroPageadressY(u32 Cycles, Memory& memory);
    Word Absolute(u32 Cycles, Memory& memory);
    Word AbsoluteX(u32 Cycles, Memory& memory);
    Word AbsoluteY(u32 Cycles, Memory& memory);
    Word Indirect(u32 Cycles, Memory& memory);
    Word Relative(u32 Cycles, Memory& memory);

    // Instruction set
    void Execute(u32 Cycles, Memory& memory);
};

#endif // CPU_H