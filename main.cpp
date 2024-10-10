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

    Byte C:1;
    Byte Z:1;
    Byte I:1;
    Byte D:1;
    Byte B:1;
    Byte V:1;
    Byte N:1;

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
    INS_LDA_IM = 0xA9,
    INS_LDA_ZP = 0xA5,
    INS_LDA_ZPX = 0xB5,
    INS_LDA_AB = 0xAD,
    INS_LDA_ABX = 0xBD,
    INS_LDA_ABY =0xB9,
    INS_LDA_INX = 0xA1,
    INS_LDX_IM = 0xA2,
    INS_JSR_AB = 0x20,
    INS_LDX_ZP = 0xA6,
    INS_LDX_ZPY = 0xB6,
    INS_LDX_AB = 0xAE,
    INS_LDX_ABY = 0xBE,
    INS_LDY_IM = 0xA0,
    INS_LDY_ZP = 0xA4,
    INS_LDY_ZPX = 0xB4,
    INS_LDY_AB = 0xAC,
    INS_LDY_ABX = 0xBC;
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
