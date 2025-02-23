#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

struct Memory{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise(){
        for(u32 i =0; i< MAX_MEM;i++)
        {
            Data[i]=0;
        }

    }
    Byte operator[]( u32 Address) const{
        return Data[Address];
    }

    Byte& operator[]( u32 Address){
        return Data[Address];
    }
    void writeWord(Word Value, u32 Address,u32& Cycles){
        Data[Address] = Value & 0xFF;
        Data[Address + 1] = (Value >> 8);
        Cycles -=2;
    }
};
struct CPU{


    Word PC;//ProgCount
    Byte SP;//StackPointer;

    Byte A ;//Accumulator
    Byte X ;//IndexRegistor
    Byte Y; //IndexRegistor

    Byte C:1;//carry flag
    Byte Z:1;//zero flag
    Byte I:1;//interrupt flag
    Byte D:1;//decimal mode flag
    Byte B:1;//break flag
    Byte V:1;//overflow flag
    Byte N:1;//negative flag

    void Reset(Memory& memory){
        PC = 0xFFFC;
        SP = 0x0100;
        C= Z = I= D = B = V =N  = 0;
        A= X =Y =0;
        memory.Initialise();
        //stack is from 0x0100 to 0x01FFl

    }
    Byte FetchByte( u32& Cycles, Memory& memory){
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }
    Word FetchWord(u32& Cycles, Memory& memory){
        Word  Data = memory[PC];
        PC++;
        Cycles--;

        Data != (memory[PC] << 8);
        PC++;
        Cycles+=2;
        //return to add handling for big endianess
        return Data;

    }

    Byte ReadByte( u32& Cycles, Byte Address,Memory& memory){
        Byte Data = memory[Address];
        Cycles--;
        return Data;

    }
    //OPCODES
    static constexpr Byte
    //LOAD operations
    INS_LDA_IM = 0xA9,
    INS_LDA_ZP = 0xA5,
    INS_LDA_ZPX = 0xB5,
    INS_LDA_AB = 0xAD,
    INS_LDA_ABX = 0xBD,
    INS_LDA_ABY =0xB9,
    INS_LDA_INX = 0xA1,
    INS_LDX_IM = 0xA2,
    INS_LDX_ZP = 0xA6,
    INS_LDX_ZPY = 0xB6,
    INS_LDX_AB = 0xAE,
    INS_LDX_ABY = 0xBE,
    INS_LDY_IM = 0xA0,
    INS_LDY_ZP = 0xA4,
    INS_LDY_ZPX = 0xB4,
    INS_LDY_AB = 0xAC,
    INS_LDY_ABX = 0xBC,
    // single jump operation
    INS_JSR_AB = 0x20,
    //store operations
    INS_STA_ZP = 0x85,
    INS_STA_ZPX = 0x95,
    INS_STA_AB = 0x8D,
    INS_STA_ABX = 0x9D,
    INS_STA_ABY = 0x99,
    INS_STA_INX = 0x81,
    INS_STA_INY = 0x91,
    INS_STX_ZP = 0x86,
    INS_STX_ZPY = 0x96,
    INS_STX_AB = 0x8E,
    INS_STY_ZP = 0x84,
    INS_STY_ZPX = 0x94,
    INS_STY_AB = 0x8C,
    // register transfer ops
    INS_TAX = 0xAA,
    INS_TAY = 0xA8,
    INS_TYA = 0x98,
    INS_TXA = 0x8A,
    //stack ops
    INS_TSX = 0xBA,
    INS_TXS = 0x9A,
    //logical operations
    INS_AND_IM = 0x29,
    INS_AND_ZP = 0x25,
    INS_AND_ZPX = 0x35,
    INS_AND_AB = 0x2D,
    INS_AND_ABX = 0x3D,
    INS_AND_ABY = 0x39,
    INS_AND_INX = 0x21,
    INS_AND_INY = 0x31,
    INS_EOR_IM = 0x49,
    INS_EOR_ZP = 0x45,
    INS_EOR_ZPX = 0x55,
    INS_EOR_AB = 0x4D,
    INS_EOR_ABX = 0x5D,
    INS_EOR_ABY = 0x59,
    //Arithmetic operations
    INS_ADC_IM = 0x69,
    INS_ADC_ZP = 0x65,
    INS_ADC_ZPX = 0x75,
    INS_ADC_AB = 0x6D,
    INS_ADC_ABX = 0x7D,
    INS_ADC_ABY = 0x79,
    INS_ADC_INX = 0x61,
    INS_ADC_INY = 0x71,
    INS_SBC_IM = 0xE9,
    INS_SBC_ZP = 0xE5,
    INS_SBC_ZPX = 0xF5,
    INS_SBC_AB = 0xED,
    INS_SBC_ABX = 0xFD,
    INS_SBC_ABY = 0xF9,
    INS_SBC_INX = 0xE1,
    INS_SBC_INY = 0xF1,
    //Increment and Decrement operations
    INS_INC_ZP = 0xE6,
    INS_INC_ZPX = 0xF6,
    INS_INC_AB = 0xEE,
    INS_INC_ABX = 0xFE,
    INS_INX = 0xE8,
    INS_INY = 0xC8,
    INS_DEC_ZP = 0xC6,
    INS_DEC_ZPX = 0xD6,
    INS_DEC_AB = 0xCE,
    INS_DEC_ABX = 0xDE,
    INS_DEX = 0xCA,
    INS_DEY = 0x88,
    //Shift operations
    INS_ASL_IM = 0x0A,
    INS_ASL_ZP = 0x06,
    INS_ASL_ZPX = 0x16,
    INS_ASL_AB = 0x0E,
    INS_ASL_ABX = 0x1E,
    INS_LSR_IM = 0x4A,
    INS_LSR_ZP = 0x46,
    INS_LSR_ZPX = 0x56,
    INS_LSR_AB = 0x4E,
    INS_LSR_ABX = 0x5E,
    INS_ROL_IM = 0x2A,
    INS_ROL_ZP = 0x26,
    INS_ROL_ZPX = 0x36,
    INS_ROL_AB = 0x2E,
    INS_ROL_ABX = 0x3E,
    INS_ROR_IM = 0x6A,
    INS_ROR_ZP = 0x66,
    INS_ROR_ZPX = 0x76,
    INS_ROR_AB = 0x6E,
    INS_ROR_ABX = 0x7E,
    //compare operations
    INS_CMP_IM = 0xC9,
    INS_CMP_ZP = 0xC5,
    INS_CMP_ZPX = 0xD5,
    INS_CMP_AB = 0xCD,
    INS_CMP_ABX = 0xDD,
    INS_CMP_ABY = 0xD9,
    INS_CMP_INX = 0xC1,
    INS_CMP_INY = 0xD1,
    INS_CPX_IM = 0xE0,
    INS_CPX_ZP = 0xE4,
    INS_CPX_AB = 0xEC,
    INS_CPY_IM = 0xC0,
    INS_CPY_ZP = 0xC4,
    INS_CPY_AB = 0xCC,
    //branch operations
    INS_BCC = 0x90,
    INS_BCS = 0xB0,
    INS_BEQ = 0xF0,
    INS_BNE = 0xD0,
    INS_BMI = 0x30,
    INS_BPL = 0x10,
    INS_BVC = 0x50,
    INS_BVS = 0x70,
    //jump operations
    INS_JMP_AB = 0x4C,
    INS_JMP_IN = 0x6C,
    INS_RTS = 0x60,
    INS_RTI = 0x40,
    //status flag operations
    INS_CLC = 0x18,
    INS_CLD = 0xD8,
    INS_CLI = 0x58,
    INS_CLV = 0xB8,
    INS_SEC = 0x38,
    INS_SED = 0xF8,
    INS_SEI = 0x78,
    //system operations
    INS_BRK = 0x00,
    INS_NOP = 0xEA,
    INS_RTI = 0x40;

    //status flag operations
    void LDRSetStatus(){
        Z = (A==0);
        N = (A & 0b10000000) > 0;
    }
    void CMPSetStatus(Byte M, Byte A){
        Z = (A - M) == 0;
        C = A >= M;
        N = (A - M) < 0;
    }
    void TRSetStatus(Byte M){
        Z = (M == 0);
        N = (M & 0b10000000) > 0;
    }
    void ADCSetStatus(Byte M){
        Z = (A + M) == 0;
        C = A + M > 255;
        N = (A + M) & 0b10000000;
        V = (A + M) > 127;
    }
    void SBCSetStatus(Byte M){
        Z = (A - M) == 0;
        C = A - M > 255;
        N = (A - M) & 0b10000000;
        V = (A - M) > 127;
    }

    //Adressing modes
    Byte ZeroPageadress(u32 Cycles,Memory& memory){
        Byte value;
        Byte ZeroPageAddress = FetchByte(Cycles, memory);
        value = ReadByte(Cycles,ZeroPageAddress, memory);
        return  value;
    }
    Byte ZeroPageadressX(u32 Cycles,Memory& memory){
        Byte value;
        Byte ZeroPageAddress = FetchByte(Cycles, memory);
        value = ReadByte(Cycles,ZeroPageAddress, memory);
        return  value + X;
    }
    Byte ZeroPageadressY(u32 Cycles,Memory& memory){
        Byte value;
        Byte ZeroPageAddress = FetchByte(Cycles, memory);
        value = ReadByte(Cycles,ZeroPageAddress, memory);
        return  value + Y;
    }
    Word Absolute(u32 Cycles,Memory& memory){
        Word AbValue = FetchWord(Cycles,memory);
        return AbValue;
    }
    Word AbsoluteX(u32 Cycles,Memory& memory){
        Word AbValue = FetchWord(Cycles,memory);
        return AbValue+X;
    }
    Word AbsoluteY(u32 Cycles,Memory& memory){
        Word AbValue = FetchWord(Cycles,memory);
        return AbValue+Y;
    }
    Word Indirect(u32 Cycles,  Memory& memory){
        Word SubAddress = FetchWord(Cycles, memory);
        PC--;
        return SubAddress;

    }
    Word Relative(u32 Cycles, Memory& memory){
        Word Offset = FetchByte(Cycles, memory);
        if(Offset & 0x80){
            Offset |= 0xFF00;
        }
        return Offset;
    }


//INSTRUCTION SET
    void Execute(u32 Cycles,Memory& memory){
        while(Cycles>0){
            Byte Ins = FetchByte(Cycles,memory);
            switch(Ins){
                case INS_LDA_IM:{
                    Byte Value = FetchByte(Cycles, memory);
                    A = Value;
                    LDRSetStatus();
                    Cycles=-2;
                }break;
                case INS_LDA_ZP:{
                   // FetchByte(Cycles, memory);  //taken out bcos increases req  number of cycles to 4 instaed of 3. doesnt seem to change anything
                   A = ZeroPageadress(Cycles,memory);
                   Cycles--;
                   LDRSetStatus();
                    //printf("running");
                }break;
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
                case INS_JSR_AB: {
                    Word SubAddress = FetchWord(Cycles, memory);
                    memory.writeWord(PC-1,SP,Cycles);
                    SP++;
                    memory[SP] = PC -1;
                    PC = SubAddress;
                    Cycles --;

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
                case INS_STA_ZP:{
                    Byte ZeroPageAddress = FetchByte(Cycles, memory);
                    memory[ZeroPageAddress] = A;
                    Cycles--;
                }break;
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
                case INS_JMP_IN: {
                    Word Address = FetchWord(Cycles, memory);
                    PC = memory[Address] | (memory[Address + 1] << 8);
                    Cycles -= 2;
                } break;
                case INS_RTS: {
                    PC = FetchWord(memory, Cycles) + 1;
                    Cycles -= 2;
                } break;
                case INS_RTI: {
                    // Implement RTI logic here
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
                case INS_BRK: {
                    // Implement BRK logic here
                } break;
                case INS_NOP: {
                    Cycles--;
                } break;
                default:{
                    printf("instruction not handled %d",Ins);
                }break;
            }
        }

    }



};

int main() {
    Memory mem;
    CPU cpu;
    cpu.Reset(mem);
    mem[0XFFFC]= CPU :: INS_LDA_INX;
    mem[0xFFFD]= 0x42;
    mem[0x0042]= CPU :: INS_LDA_IM;
    mem[0x4243]= 0x84;
    cpu.Execute(6,mem);
    return 0;

}
