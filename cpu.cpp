#include "cpu.h"

void Memory::Initialise() {
    for (u32 i = 0; i < MAX_MEM; i++) {
        Data[i] = 0;
    }
}

Byte Memory::operator[](u32 Address) const {
    return Data[Address];
}

Byte& Memory::operator[](u32 Address) {
    return Data[Address];
}

void Memory::writeWord(Word Value, u32 Address, u32& Cycles) {
    Data[Address] = Value & 0xFF;
    Data[Address + 1] = (Value >> 8);
    Cycles -= 2;
}


void CPU::Reset(Memory& memory) {
    PC = 0xFFFC;
    SP = 0x0100;
    C = Z = I = D = B = V = N = 0;
    A = X = Y = 0;
    memory.Initialise();
}

Byte CPU::FetchByte(u32& Cycles, Memory& memory) {
    Byte Data = memory[PC];
    PC++;
    Cycles--;
    return Data;
}

Word CPU::FetchWord(u32& Cycles, Memory& memory) {
    Word Data = memory[PC];
    PC++;
    Cycles--;
    Data |= (memory[PC] << 8);
    PC++;
    Cycles--;
    return Data;
}

Byte CPU::ReadByte(u32& Cycles, Byte Address, Memory& memory) {
    Byte Data = memory[Address];
    Cycles--;
    return Data;
}


Byte CPU::ZeroPageadress(u32 Cycles, Memory& memory) {
    Byte Address = FetchByte(Cycles, memory);
    Cycles--;
    return Address;
}

Byte CPU::ZeroPageadressX(u32 Cycles, Memory& memory) {
    Byte Address = FetchByte(Cycles, memory);
    Address += X;
    Cycles--;
    return Address;
}

Byte CPU::ZeroPageadressY(u32 Cycles, Memory& memory) {
    Byte Address = FetchByte(Cycles, memory);
    Address += Y;
    Cycles--;
    return Address;
}

Word CPU::Absolute(u32 Cycles, Memory& memory) {
    Word Address = FetchWord(Cycles, memory);
    return Address;
}

Word CPU::AbsoluteX(u32 Cycles, Memory& memory) {
    Word Address = FetchWord(Cycles, memory);
    Address += X;
    Cycles--;
    return Address;
}

Word CPU::AbsoluteY(u32 Cycles, Memory& memory) {
    Word Address = FetchWord(Cycles, memory);
    Address += Y;
    Cycles--;
    return Address;
}

Word CPU::Indirect(u32 Cycles, Memory& memory) {
    Word Address = FetchWord(Cycles, memory);
    Address = memory[Address] | (memory[Address + 1] << 8);
    Cycles--;
    return Address;
}

Word CPU::Relative(u32 Cycles, Memory& memory) {
    Byte Offset = FetchByte(Cycles, memory);
    if (Offset & 0x80) {
        Offset |= 0xFF00;
    }
    Cycles--;
    return Offset;
}
// Status flag operations
void CPU::LDRSetStatus() {
    Z = (A == 0);
    N = (A & 0x80) > 0;
}

void CPU::CMPSetStatus(Byte M, Byte A) {
    Z = (A == M);
    N = ((A - M) & 0x80) > 0;
    C = (A >= M);
}

void CPU::TRSetStatus(Byte M) {
    Z = (M == 0);
    N = (M & 0x80) > 0;
}

void CPU::ADCSetStatus(Byte M) {
    Z = (A + M) == 0;
    C = A + M > 255;
    N = (A + M) & 0b10000000;
    V = (A + M) > 127;
}

void CPU::SBCSetStatus(Byte M) {
    Z = (A - M) == 0;
    C = A - M >= 0;
    N = (A - M) & 0b10000000;
    V = (A - M) > 127;
}

void CPU::Execute(u32 Cycles, Memory& memory) {
    this->Cycles = Cycles;
    while (Cycles > 0) {
        Byte Ins = FetchByte(Cycles, memory);
        switch (Ins) {
            case INS_LDA_IM: {
                Byte Value = FetchByte(Cycles, memory);
                A = Value;
                LDRSetStatus();
            } break;
            case INS_LDA_ZP: {
                A = ZeroPageadress(Cycles, memory);
                LDRSetStatus();
            } break;
            case INS_LDA_ZPX:{
                Cycles--;
                A = ZeroPageadressX(Cycles, memory);
            }break;
            case INS_LDA_INX:{
                Word SubAddress = Indirect(Cycles, memory );
                memory[PC] = SubAddress + X;
                //Ins = FetchByte(Cycles,memory);
            }break;
            case INS_LDA_AB:{
                A = Absolute(Cycles,memory);
                LDRSetStatus();
            }break;
            case INS_LDA_ABX: {
                A = AbsoluteX(Cycles,memory);
                LDRSetStatus();
            }break;
            case INS_LDA_ABY: {
                A = AbsoluteY(Cycles,memory);
                LDRSetStatus();

            }break;
            case INS_LDX_IM:{
                Byte Value = FetchByte(Cycles, memory);
                X = Value;
                LDRSetStatus();
            }break;
            case INS_LDX_ZP:{
                // FetchByte(Cycles, memory);  //taken out bcos increases req  number of cycles to 4 instaed of 3. doesnt seem to change anything
                X = ZeroPageadress(Cycles,memory);
                LDRSetStatus();

            }break;
            case INS_LDX_ZPY:{
                X = ZeroPageadressY(Cycles, memory);
                LDRSetStatus();
            }break;
            case INS_LDX_AB:{
                X = Absolute(Cycles, memory);
                LDRSetStatus();
            }break;
            case INS_LDX_ABY: {
                X = AbsoluteY(Cycles, memory);
                LDRSetStatus();
            }break;
            case INS_LDY_IM:{
                Byte Value = FetchByte(Cycles, memory);
                Y = Value;
                LDRSetStatus();
            }break;
            case INS_LDY_ZP:{
                Y = ZeroPageadress(Cycles, memory);
                LDRSetStatus();

            }break;
            case INS_LDY_ZPX:{
                Y = ZeroPageadress(Cycles, memory);
                LDRSetStatus();
            }break;
            case INS_LDY_AB:{
                Y = Absolute(Cycles, memory);
                LDRSetStatus();
            }break;
            case INS_LDY_ABX:{
                Y = AbsoluteX(Cycles, memory);
                LDRSetStatus();
            }break;

            // Store operations
            case INS_STA_ZP: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress] = A;
                Cycles--;
            } break;
            case INS_STA_ZPX:{
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress + X] = A;
                Cycles--;
            }break;
            case INS_STA_AB:{
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue] = A;
                Cycles-=2;
            }break;
            case INS_STA_ABX:{
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue + X] = A;
                Cycles-=2;
            }break;
            case INS_STA_ABY:{
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue + Y] = A;
                Cycles-=2;
            }break;
            case INS_STA_INX:{
                Word SubAddress = Indirect(Cycles, memory);
                memory[SubAddress + X] = A;

            }break;
            case INS_STA_INY:{
                Word SubAddress = Indirect(Cycles, memory);
                memory[SubAddress + Y] = A;
            }break;
            case INS_STX_ZP:{
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress] = X;
                Cycles--;
            }break;
            case INS_STX_ZPY:{
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress + Y] = X;
                Cycles--;
            }break;
            case INS_STX_AB:{
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue] = X;
                Cycles-=2;
            }break;
            case INS_STY_ZP:{
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress] = Y;
                Cycles--;
            }break;
            case INS_STY_ZPX:{
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress + X] = Y;
                Cycles--;
            }break;
            case INS_STY_AB:{
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue] = Y;
                Cycles-=2;
            }break;

            // Transfer operations
            case INS_TAX:{
                X = A;
                TRSetStatus(X);
            }break;
            case INS_TAY:{
                Y = A;
                TRSetStatus(Y);
            }break;
            case INS_TSX:{
                X = SP;
                TRSetStatus(X);
            }break;
            case INS_TXA:{
                A = X;
                TRSetStatus(A);
            }break;
            case INS_TXS:{
                SP = X;
            }break;
            case INS_TYA:{
                A = Y;
                TRSetStatus(A);
            }break;

            // Bitwise operations
            case INS_AND_IM:{
                Byte M = FetchByte(Cycles, memory);
                A = A & M;
                LDRSetStatus();
            }break;
            case INS_AND_ZP:{
                Byte M = ZeroPageadress(Cycles, memory);
                A = A & M;
                LDRSetStatus();
            }break;
            case INS_AND_ZPX:{
                Byte M = ZeroPageadressX(Cycles, memory);
                A = A & M;
                LDRSetStatus();
            }break;
            case INS_AND_AB:{
                Byte M = Absolute(Cycles, memory);
                A = A & M;
                LDRSetStatus();
            }break;
            case INS_AND_ABX:{
                Byte M = AbsoluteX(Cycles, memory);
                A = A & M;
                LDRSetStatus();
            }break;
            case INS_AND_ABY:{
                Byte M = AbsoluteY(Cycles, memory);
                A = A & M;
                LDRSetStatus();
            }break;
            case INS_AND_INX:{
                Byte M = Indirect(Cycles, memory);
                A = A & M;
                LDRSetStatus();
            }break;
            case INS_AND_INY:{
                Byte M = Indirect(Cycles, memory);
                A = A & M;
                LDRSetStatus();
            }break;
            case INS_EOR_IM:{
                Byte M = FetchByte(Cycles, memory);
                A = A ^ M;
                LDRSetStatus();
            }break;
            case INS_EOR_ZP:{
                Byte M = ZeroPageadress(Cycles, memory);
                A = A ^ M;
                LDRSetStatus();
            }break;
            case INS_EOR_ZPX:{
                Byte M = ZeroPageadressX(Cycles, memory);
                A = A ^ M;
                LDRSetStatus();
            }break;
            case INS_EOR_AB:{
                Byte M = Absolute(Cycles, memory);
                A = A ^ M;
                LDRSetStatus();
            }break;
            case INS_EOR_ABX:{
                Byte M = AbsoluteX(Cycles, memory);
                A = A ^ M;
                LDRSetStatus();
            }break;
            case INS_EOR_ABY:{
                Byte M = AbsoluteY(Cycles, memory);
                A = A ^ M;
                LDRSetStatus();
            }break;
            case INS_EOR_INX:{
                Byte M = Indirect(Cycles, memory);
                A = A ^ M;
                LDRSetStatus();
            }break;
            case INS_EOR_INY:{
                Byte M = Indirect(Cycles, memory);
                A = A ^ M;
                LDRSetStatus();
            }break;
            case INS_ORA_IM:{
                Byte M = FetchByte(Cycles, memory);
                A = A | M;
                LDRSetStatus();
            }break;
            case INS_ORA_ZP:{
                Byte M = ZeroPageadress(Cycles, memory);
                A = A | M;
                LDRSetStatus();
            }break;
            case INS_ORA_ZPX:{
                Byte M = ZeroPageadressX(Cycles, memory);
                A = A | M;
                LDRSetStatus();
            }break;
            case INS_ORA_AB:{
                Byte M = Absolute(Cycles, memory);
                A = A | M;
                LDRSetStatus();
            }break;
            case INS_ORA_ABX:{
                Byte M = AbsoluteX(Cycles, memory);
                A = A | M;
                LDRSetStatus();
            }break;
            case INS_ORA_ABY:{
                Byte M = AbsoluteY(Cycles, memory);
                A = A | M;
                LDRSetStatus();
            }break;
            case INS_ORA_INX:{
                Byte M = Indirect(Cycles, memory);
                A = A | M;
                LDRSetStatus();
            }break;
            case INS_ORA_INY:{
                Byte M = Indirect(Cycles, memory);
                A = A | M;
                LDRSetStatus();
            }break;

                // Arithmetic operations
            case INS_ADC_IM:{
                Byte M = FetchByte(Cycles, memory);
                A = A + M + C;
                ADCSetStatus(M);
            }break;
            case INS_ADC_ZP:{
                Byte M = ZeroPageadress(Cycles, memory);
                A = A + M + C;
                ADCSetStatus(M);
            }break;
            case INS_ADC_ZPX:{
                Byte M = ZeroPageadressX(Cycles, memory);
                A = A + M + C;
                ADCSetStatus(M);
            }break;
            case INS_ADC_AB:{
                Byte M = Absolute(Cycles, memory);
                A = A + M + C;
                ADCSetStatus(M);
            }break;
            case INS_ADC_ABX:{
                Byte M = AbsoluteX(Cycles, memory);
                A = A + M + C;
                ADCSetStatus(M);
            }break;
            case INS_ADC_ABY:{
                Byte M = AbsoluteY(Cycles, memory);
                A = A + M + C;
                ADCSetStatus(M);
            }break;
            case INS_ADC_INX:{
                Byte M = Indirect(Cycles, memory);
                A = A + M + C;
                ADCSetStatus(M);
            }break;
            case INS_ADC_INY:{
                Byte M = Indirect(Cycles, memory);
                A = A + M + C;
                ADCSetStatus(M);
            }break;
            case INS_SBC_IM:{
                Byte M = FetchByte(Cycles, memory);
                A = A - M - C;
                SBCSetStatus(M);
            }break;
            case INS_SBC_ZP:{
                Byte M = ZeroPageadress(Cycles, memory);
                A = A - M - C;
                SBCSetStatus(M);
            }break;
            case INS_SBC_ZPX:{
                Byte M = ZeroPageadressX(Cycles, memory);
                A = A - M - C;
                SBCSetStatus(M);
            }break;
            case INS_SBC_AB:{
                Byte M = Absolute(Cycles, memory);
                A = A - M - C;
                SBCSetStatus(M);
            }break;
            case INS_SBC_ABX:{
                Byte M = AbsoluteX(Cycles, memory);
                A = A - M - C;
                SBCSetStatus(M);
            }break;
            case INS_SBC_ABY:{
                Byte M = AbsoluteY(Cycles, memory);
                A = A - M - C;
                SBCSetStatus(M);
            }break;
            case INS_SBC_INX:{
                Byte M = Indirect(Cycles, memory);
                A = A - M - C;
                SBCSetStatus(M);
            }break;
            case INS_SBC_INY:{
                Byte M = Indirect(Cycles, memory);
                A = A - M - C;
                SBCSetStatus(M);
            }break;

            // Increment and Decrement operations
            case INS_INC_ZP: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress]++;
                Cycles--;
            } break;
            case INS_INC_ZPX: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress + X]++;
                Cycles--;
            } break;
            case INS_INC_AB: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue]++;
                Cycles -= 2;
            } break;
            case INS_INC_ABX: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue + X]++;
                Cycles -= 2;
            } break;
            case INS_INX: {
                X++;
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_INY: {
                Y++;
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_DEC_ZP: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress]--;
                Cycles--;
            } break;
            case INS_DEC_ZPX: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress + X]--;
                Cycles--;
            } break;
            case INS_DEC_AB: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue]--;
                Cycles -= 2;
            } break;
            case INS_DEC_ABX: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue + X]--;
                Cycles -= 2;
            } break;
            case INS_DEX: {
                X--;
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_DEY: {
                Y--;
                LDRSetStatus();
                Cycles--;
            } break;
                // Shift operations
            case INS_ASL_IM: {
                A = A << 1;
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_ASL_ZP: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress] = memory[ZeroPageAddress] << 1;
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_ASL_ZPX: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress + X] = memory[ZeroPageAddress + X] << 1;
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_ASL_AB: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue] = memory[AbValue] << 1;
                LDRSetStatus();
                Cycles -= 2;
            } break;
            case INS_ASL_ABX: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue + X] = memory[AbValue + X] << 1;
                LDRSetStatus();
                Cycles -= 2;
            } break;
            case INS_LSR_IM: {
                A = A >> 1;
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_LSR_ZP: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress] = memory[ZeroPageAddress] >> 1;
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_LSR_ZPX: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress + X] = memory[ZeroPageAddress + X] >> 1;
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_LSR_AB: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue] = memory[AbValue] >> 1;
                LDRSetStatus();
                Cycles -= 2;
            } break;
            case INS_LSR_ABX: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue + X] = memory[AbValue + X] >> 1;
                LDRSetStatus();
                Cycles -= 2;
            } break;
            case INS_ROL_IM: {
                A = (A << 1) | (A >> 7);
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_ROL_ZP: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress] = (memory[ZeroPageAddress] << 1) | (memory[ZeroPageAddress] >> 7);
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_ROL_ZPX: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress + X] = (memory[ZeroPageAddress + X] << 1) | (memory[ZeroPageAddress + X] >> 7);
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_ROL_AB: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue] = (memory[AbValue] << 1) | (memory[AbValue] >> 7);
                LDRSetStatus();
                Cycles -= 2;
            } break;
            case INS_ROL_ABX: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue + X] = (memory[AbValue + X] << 1) | (memory[AbValue + X] >> 7);
                LDRSetStatus();
                Cycles -= 2;
            } break;
            case INS_ROR_IM: {
                A = (A >> 1) | (A << 7);
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_ROR_ZP: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress] = (memory[ZeroPageAddress] >> 1) | (memory[ZeroPageAddress] << 7);
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_ROR_ZPX: {
                Byte ZeroPageAddress = FetchByte(Cycles, memory);
                memory[ZeroPageAddress + X] = (memory[ZeroPageAddress + X] >> 1) | (memory[ZeroPageAddress + X] << 7);
                LDRSetStatus();
                Cycles--;
            } break;
            case INS_ROR_AB: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue] = (memory[AbValue] >> 1) | (memory[AbValue] << 7);
                LDRSetStatus();
                Cycles -= 2;
            } break;
            case INS_ROR_ABX: {
                Word AbValue = FetchWord(Cycles, memory);
                memory[AbValue + X] = (memory[AbValue + X] >> 1) | (memory[AbValue + X] << 7);
                LDRSetStatus();
                Cycles -= 2;
            } break;

                // Compare operations
            case INS_CMP_IM: {
                Byte M = FetchByte(Cycles, memory);
                CMPSetStatus(M, A);
                Cycles--;

            } break;
            case INS_CMP_ZP: {
                Byte M = ZeroPageadress(Cycles, memory);
                CMPSetStatus(M, A);
                Cycles--;


            } break;
            case INS_CMP_ZPX: {
                Byte M = ZeroPageadressX(Cycles, memory);
                CMPSetStatus(M, A);
                Cycles--;
            } break;
            case INS_CMP_AB: {
                Byte M = Absolute(Cycles, memory);
                CMPSetStatus(M, A);
            } break;
            case INS_CMP_ABX: {
                Byte M = AbsoluteX(Cycles, memory);
                CMPSetStatus(M, A);

            } break;
            case INS_CMP_ABY: {
                Byte M = AbsoluteY(Cycles, memory);
                CMPSetStatus(M, A);
            } break;
            case INS_CMP_INX: {
                Byte M = Indirect(Cycles, memory);
                CMPSetStatus(M, A);
            } break;
            case INS_CMP_INY: {
                Byte M = Indirect(Cycles, memory);
                CMPSetStatus(M, A);
                Cycles--;
            } break;
            case INS_CPX_IM: {
                Byte M = FetchByte(Cycles, memory);
                CMPSetStatus(M, X);
                Cycles--;
            } break;
            case INS_CPX_ZP: {
                Byte M = ZeroPageadress(Cycles, memory);
                CMPSetStatus(M, X);
                Cycles--;
            } break;
            case INS_CPX_AB: {
                Byte M = Absolute(Cycles, memory);
                CMPSetStatus(M, X);
                Cycles -= 2;
            } break;
            case INS_CPY_IM: {
                Byte M = FetchByte(Cycles, memory);
                CMPSetStatus(M, Y);
                Cycles--;
            } break;
            case INS_CPY_ZP: {
                Byte M = ZeroPageadress(Cycles, memory);
                CMPSetStatus(M, Y);
                Cycles--;
            } break;
            case INS_CPY_AB: {
                Byte M = Absolute(Cycles, memory);
                CMPSetStatus(M, Y);
                Cycles -= 2;
            } break;

                // Branch operations
            case INS_BCC: {
                Byte Offset = Relative(Cycles, memory);
                if (C == 0) {
                    PC += Offset;
                    Cycles--;
                }
            } break;
            case INS_BCS: {
                Byte Offset = Relative(Cycles, memory);
                if (C == 1) {
                    PC += Offset;
                    Cycles--;
                }
            } break;
            case INS_BEQ: {
                Byte Offset = Relative(Cycles, memory);
                if (Z == 1) {
                    PC += Offset;
                    Cycles--;
                }
            } break;
            case INS_BNE: {
                Byte Offset = Relative(Cycles, memory);
                if (Z == 0) {
                    PC += Offset;
                    Cycles--;
                }
            } break;
            case INS_BMI: {
                Byte Offset = Relative(Cycles, memory);
                if (N == 1) {
                    PC += Offset;
                    Cycles--;
                }
            } break;
            case INS_BPL: {
                Byte Offset = Relative(Cycles, memory);
                if (N == 0) {
                    PC += Offset;
                    Cycles--;
                }
            } break;
            case INS_BVC: {
                Byte Offset = Relative(Cycles, memory);
                if (V == 0) {
                    PC += Offset;
                    Cycles--;
                }
            } break;
            case INS_BVS: {
                Byte Offset = Relative(Cycles, memory);
                if (V == 1) {
                    PC += Offset;
                    Cycles--;
                }
            } break;

                // Jump operations
            case INS_JMP_AB: {
                Word Address = FetchWord(Cycles, memory);
                PC = Address;
                Cycles -= 2;
            } break;

                // Status flag operations
            case INS_CLC: {
                C = 0;
                Cycles--;
            } break;
            case INS_CLD: {
                D = 0;
                Cycles--;
            } break;
            case INS_CLI: {
                I = 0;
                Cycles--;
            } break;
            case INS_CLV: {
                V = 0;
                Cycles--;
            } break;
            case INS_SEC: {
                C = 1;
                Cycles--;
            } break;
            case INS_SED: {
                D = 1;
                Cycles--;
            } break;
            case INS_SEI: {
                I = 1;
                Cycles--;
            } break;

                // System operations




            default: {
                printf("Instruction not handled %d", Ins);
            } break;
        }
    }
}