Computer Architecture Projects


This repository contains all assignments completed during the Computer Architecture course offered in the 1st semester of 2nd year (Spring 2024) at Dankook University.

Each project incrementally builds understanding of modern processor design, from basic register-based execution to pipelined MIPS architecture with cache simulation.


Project Overview
  Assignment	Title	Description
  1	Register-Based Calculator	Simple interpreter that parses and executes basic instructions on virtual registers
  2	Single-Cycle MIPS Emulator	Simulates a MIPS processor where each instruction completes in one clock cycle
  3	Pipelined MIPS Emulator	Implements a 5-stage pipeline (IF-ID-EX-MEM-WB) with hazard resolution
  4	Pipelined MIPS with Cache Simulation	Adds data cache simulation to the pipelined MIPS emulator with AMAT calculation

---

Assignment Summaries


Assignment 1: Register-Based Calculator
*  Implements 32 virtual registers (r0 ~ r31)
*  Supports basic arithmetic and data movement: add, sub, mul, div, mov, halt
*  Final register state is printed after interpreting the given instruction set


Build & Run:
  gcc -o calc main.c
  ./calc input.txt


Assignment 2: Single-Cycle MIPS Emulator
* Emulates MIPS ISA where each instruction completes in one clock cycle
* Memory and register file simulated with arrays
* Input is a binary file generated from .c or .asm via mips-linux-gnu toolchain
* PC stops execution at 0xFFFFFFFF

Output includes:
* Instruction count
* Branch statistics
* Register and memory access logs


Assignment 3: Pipelined MIPS Emulator
* Introduces 5-stage pipeline: IF → ID → EX → MEM → WB
* Implements forwarding and stall logic to resolve data hazards
* Includes basic control hazard handling
* Displays architectural state per cycle

Performance Metrics:
* Total cycles vs. single-cycle
* Instruction throughput
* Instruction class statistics


Assignment 4: Pipelined MIPS with Cache
* Adds configurable cache model (block size, associativity, capacity)
* Simulates write-back and write-through policies
* Tracks cache hits/misses, cold/conflict misses
* Calculates AMAT (Average Memory Access Time)

Cache Settings:
* Line size: 64 bytes
* Associativity: 1-way (direct), 2-way, 4-way...
* Memory latency: 1000 cycles, cache latency: 1 cycle

---

Development & Environment
* Language: C (GCC)
* Build System: Manual compilation via gcc
* MIPS toolchain: mips-linux-gnu-gcc, mips-linux-gnu-objcopy
* Input format: .txt (for calculator), .bin (for MIPS simulators)

---
Learning Objectives
Through these progressive assignments, students gained experience in:

* Instruction parsing and virtual machine simulation
* Understanding MIPS ISA and datapath
* Pipeline architecture design and control hazard mitigation
* Memory hierarchy concepts and cache modeling
* Performance evaluation using metrics such as CPI and AMAT

---

Repository Structure (Recommended)
.
├── Assignment1_RegisterCalculator/
│   ├── main.c
│   ├── input.txt
│   └── README.md
├── Assignment2_SingleCycleMIPS/
│   ├── mips_single.c
│   ├── test.bin
│   └── README.md
├── Assignment3_PipelinedMIPS/
│   ├── mips_pipeline.c
│   ├── input.bin
│   └── README.md
├── Assignment4_MIPSWithCache/
│   ├── mips_cache_pipeline.c
│   ├── input.bin
│   └── README.md
└── README.md   ← (this file)

---

Course Information
* Course: Computer Architecture (컴퓨터구조)
* Semester: 2nd Year, 1st Semester (Spring 2024)
* University: Dankook University
* Department: Mobile System Engineering

