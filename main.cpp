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
    // single jump operation
    INS_JSR_AB = 0x20,
    INS_LDX_ZP = 0xA6,
    INS_LDX_ZPY = 0xB6,
    INS_LDX_AB = 0xAE,
    INS_LDX_ABY = 0xBE,
    INS_LDY_IM = 0xA0,
    INS_LDY_ZP = 0xA4,
    INS_LDY_ZPX = 0xB4,
    INS_LDY_AB = 0xAC,
    INS_LDY_ABX = 0xBC,
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
    INS_SBC_IM = 0xE9;

    ;


    void LDRSetStatus(){
        Z = (A==0);
        N = (A & 0b10000000) > 0;
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


//INSTRUCTION SET
    void Execute(u32 Cycles,Memory& memory){
        while(Cycles>0){
            Byte Ins = FetchByte(Cycles,memory);
            switch(Ins){
                case INS_LDA_IM:{
                    Byte Value = FetchByte(Cycles, memory);
                    A = Value;
                    LDRSetStatus();
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
                    LDRSetStatus();
                }break;
                case INS_TAY:{
                    Y = A;
                    LDRSetStatus();
                }break;
                case INS_TSX:{
                    X = SP;
                    LDRSetStatus();
                }break;
                case INS_TXA:{
                    A = X;
                    LDRSetStatus();
                }break;
                case INS_TXS:{
                    SP = X;
                }break;
                case INS_TYA:{
                    A = Y;
                    LDRSetStatus();
                }break;
                case INS_ADC_IM{
                    Byte M = FetchByte(Cycles, memory);

                }
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
