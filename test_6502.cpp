#include <gtest/gtest.h>
#include "cpu.h"  // Include your CPU and Memory class headers


class CPU6502Test : public ::testing::Test {
protected:
    Memory mem;
    CPU cpu;

    void SetUp() override {
        cpu.Reset(mem);
    }
};

TEST_F(CPU6502Test, LDAImmediate) {
mem[0xFFFC] = CPU::INS_LDA_IM;
mem[0xFFFD] = 0x84;
cpu.Execute(2, mem);
EXPECT_EQ(cpu.A, 0x84);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, LDAZeroPageX) {
mem[0xFFFC] = CPU::INS_LDA_ZPX;
mem[0xFFFD] = 0x42;
mem[0x0042] = 0x84;
cpu.X = 0x02;
cpu.Execute(4, mem);
EXPECT_EQ(cpu.A, 0x84);
EXPECT_EQ(cpu.Cycles, 0);
}


TEST_F(CPU6502Test, LDAAbsolute) {
    mem[0xFFFC] = CPU::INS_LDA_AB;
    mem[0xFFFD] = 0x00;
    mem[0xFFFE] = 0x80;
    mem[0x8000] = 0x84;
    cpu.Execute(4, mem);
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, LDAAbsoluteX) {
    mem[0xFFFC] = CPU::INS_LDA_ABX;
    mem[0xFFFD] = 0x00;
    mem[0xFFFE] = 0x80;
    mem[0x8002] = 0x84;
    cpu.X = 0x02;
    cpu.Execute(4, mem);
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, LDAAbsoluteY) {
    mem[0xFFFC] = CPU::INS_LDA_ABY;
    mem[0xFFFD] = 0x00;
    mem[0xFFFE] = 0x80;
    mem[0x8002] = 0x84;
    cpu.Y = 0x02;
    cpu.Execute(4, mem);
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, LDAIndirectX) {
    mem[0xFFFC] = CPU::INS_LDA_INX;
    mem[0xFFFD] = 0x02;
    mem[0x0004] = 0x00;
    mem[0x0005] = 0x80;
    mem[0x8000] = 0x84;
    cpu.X = 0x02;
    cpu.Execute(6, mem);
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, LDAIndirectY) {
    mem[0xFFFC] = CPU::INS_LDA_INY;
    mem[0xFFFD] = 0x02;
    mem[0x0002] = 0x00;
    mem[0x0003] = 0x80;
    mem[0x8002] = 0x84;
    cpu.Y = 0x02;
    cpu.Execute(5, mem);
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, LDXImmediate) {
mem[0xFFFC] = CPU::INS_LDX_IM;
mem[0xFFFD] = 0x84;
cpu.Execute(2, mem);
EXPECT_EQ(cpu.X, 0x84);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, LDYImmediate) {
mem[0xFFFC] = CPU::INS_LDY_IM;
mem[0xFFFD] = 0x84;
cpu.Execute(2, mem);
EXPECT_EQ(cpu.Y, 0x84);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, LDAZeroPage) {
mem[0xFFFC] = CPU::INS_LDA_ZP;
mem[0xFFFD] = 0x42;
mem[0x0042] = 0x84;
cpu.Execute(3, mem);
EXPECT_EQ(cpu.A, 0x84);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, STAZeroPage) {
cpu.A = 0x84;
mem[0xFFFC] = CPU::INS_STA_ZP;
mem[0xFFFD] = 0x42;
cpu.Execute(3, mem);
EXPECT_EQ(mem[0x0042], 0x84);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, JMPAbsolute) {
mem[0xFFFC] = CPU::INS_JMP_AB;
mem[0xFFFD] = 0x00;
mem[0xFFFE] = 0x80;
cpu.Execute(3, mem);
EXPECT_EQ(cpu.PC, 0x8000);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, SEI) {
mem[0xFFFC] = CPU::INS_SEI;
cpu.Execute(2, mem);
EXPECT_EQ(cpu.I, 1);
EXPECT_EQ(cpu.Cycles, 0);
}

// Add more test cases for other instructions...

TEST_F(CPU6502Test, CLD) {
mem[0xFFFC] = CPU::INS_CLD;
cpu.Execute(2, mem);
EXPECT_EQ(cpu.D, 0);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, SED) {
mem[0xFFFC] = CPU::INS_SED;
cpu.Execute(2, mem);
EXPECT_EQ(cpu.D, 1);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, CLC) {
mem[0xFFFC] = CPU::INS_CLC;
cpu.Execute(2, mem);
EXPECT_EQ(cpu.C, 0);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, SEC) {
mem[0xFFFC] = CPU::INS_SEC;
cpu.Execute(2, mem);
EXPECT_EQ(cpu.C, 1);
EXPECT_EQ(cpu.Cycles, 0);
}

TEST_F(CPU6502Test, CLV) {
mem[0xFFFC] = CPU::INS_CLV;
cpu.Execute(2, mem);
EXPECT_EQ(cpu.V, 0);
EXPECT_EQ(cpu.Cycles, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}