# Assignment 2: Single-Cycle MIPS Emulator

This project emulates a basic single-cycle MIPS processor. Each instruction completes in one clock cycle.

## Features

- MIPS instruction fetch → decode → execute
- Memory and register simulation using arrays
- PC initialized at 0x00000000
- Halt condition: `PC == 0xFFFFFFFF`

## Instructions

- Use `mips-linux-gnu-gcc` to compile your own `.bin`
- Use `mips-linux-gnu-objcopy` to extract `.text` section
- Load binary via array and simulate one cycle per instruction

## Metrics Output

- Total instruction count
- Branch count
- Register/memory operations
