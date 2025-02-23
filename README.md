# Project Description
This project is an emulator for the 6502 microprocessor, written in C++. 
The 6502 is a classic 8-bit microprocessor used in many early computers, gaming consoles, and other devices.
The emulator aims to replicate the behavior of the 6502 CPU, including its instruction set and memory management.

# Current State
The project is currently in the final stages of development. The following components have been implemented:  
* Memory Management: A Memory struct that simulates the 64KB memory space of the 6502.
* CPU Core: A CPU struct that includes the program counter (PC), stack pointer (SP), registers (A, X, Y), and status flags (C, Z, I, D, B, V, N).
* Instruction Set: The complete 6502 instruction set has been implemented, including load/store operations, arithmetic operations, logical operations, and branching.

## Key Features Implemented
* Memory Initialization: The memory is initialized to zero.
* Instruction Fetching: The CPU can fetch and execute instructions from memory.
* Addressing Modes: Various addressing modes such as immediate, zero-page, absolute, and indexed addressing are implemented.
* Status Flags: The CPU correctly sets and clears status flags based on the results of operations.

## Known Issues
* There may be bugs in the handling of certain instructions or edge cases.
* The emulator currently lacks a comprehensive test suite to verify correctness.

## Branches
* Main Branch: All code is contained in a single file.
* Split Branch: The code has been refactored into a proper file structure suitable for a C++ program.

## Goals
The primary goal of this project is to create a fully functional 6502 emulator that can accurately execute 6502 machine code. This includes:
* Ensuring accurate emulation of all addressing modes.
* Handling all status flags correctly.
* Providing a robust testing framework to verify the emulator's accuracy.

## Future Work
* Testing: Develop a comprehensive set of tests to ensure the emulator's correctness.
* Optimization: Improve the performance and efficiency of the emulator.
* Documentation: Provide detailed documentation and usage examples.
