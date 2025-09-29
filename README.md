Dankook University, 2nd Year 1st Semester (Spring 2024)

# Computer Architecture Projects

This repository contains all assignments from the **Computer Architecture** course, completed during the Spring 2024 semester at Dankook University. The projects progressively build an understanding of processor design, from a simple register-based calculator to a pipelined MIPS architecture with cache simulation.

---

## Project Summaries

### Assignment 1: Register-Based Calculator
- **Objective**: Implement a simple interpreter that parses and executes basic instructions on virtual registers.
- **Functionality**: Implements 32 virtual registers (`r0` to `r31`) and supports basic arithmetic (`add`, `sub`, `mul`, `div`), data movement (`mov`), and a termination instruction (`halt`). It prints the final register state after execution.
- **Build & Run**: Use `gcc -o calc main.c` and `./calc input.txt`.

### Assignment 2: Single-Cycle MIPS Emulator
- **Objective**: Emulate a MIPS processor where each instruction completes in one clock cycle.
- **Functionality**: Simulates the MIPS Instruction Set Architecture (ISA) using arrays for memory and the register file. It takes a binary file as input and outputs the instruction count, branch statistics, and register/memory access logs. Execution stops at `0xFFFFFFFF`.

### Assignment 3: Pipelined MIPS Emulator
- **Objective**: Implement a 5-stage pipeline (IF → ID → EX → MEM → WB) with hazard resolution.
- **Functionality**: Introduces **forwarding** and **stall** logic to resolve data hazards, and includes basic control hazard handling. The architectural state is displayed for each cycle.
- **Performance Metrics**: The output provides metrics such as total cycles versus single-cycle execution, instruction throughput, and instruction class statistics.

### Assignment 4: Pipelined MIPS with Cache Simulation
- **Objective**: Add a configurable data cache model to the pipelined MIPS emulator.
- **Functionality**: Simulates cache behavior with customizable line size (e.g., 64 bytes), associativity (e.g., 1-way, 2-way), and policies (write-back/write-through). It tracks cache hits/misses and calculates **AMAT (Average Memory Access Time)**.
- **Cache Settings**: Line size: 64 bytes, Memory latency: 1000 cycles, Cache latency: 1 cycle.

---

## Development & Environment

- **Language**: C (GCC)
- **Build System**: Manual compilation via `gcc`.
- **MIPS Toolchain**: `mips-linux-gnu-gcc` and `mips-linux-gnu-objcopy` for generating binary input files.
- **Input Format**: `.txt` for the calculator and `.bin` for the MIPS simulators.

---

## Learning Objectives

Through these projects, students gained experience in:
- Instruction parsing and virtual machine simulation.
- Understanding the MIPS ISA and datapath design.
- Pipelined architecture design, including hazard resolution.
- Memory hierarchy concepts and cache modeling.
- Performance evaluation using metrics like **CPI** and **AMAT**.

---

## Repository Structure

| File/Directory | Description |
| :--- | :--- |
| **Assignment1_RegisterCalculator/** | Directory for the Register Calculator project. |
| ├── `main.c` | The core C code for the calculator logic. |
| ├── `input.txt` | The input data file used by the calculator. |
| └── `README.md` | Provides detailed instructions and information for Assignment 1. |
| **Assignment2_SingleCycleMIPS/** | Directory for the Single-Cycle MIPS simulator. |
| ├── `mips_single.c` | The C implementation of the MIPS processor. |
| ├── `test.bin` | The binary file used to test the processor. |
| └── `README.md` | Provides detailed instructions and information for Assignment 2. |
| **Assignment3_PipelinedMIPS/** | Directory for the Pipelined MIPS simulator. |
| ├── `mips_pipeline.c` | The C implementation of the pipelined MIPS processor. |
| ├── `input.bin` | The binary input file used for testing. |
| └── `README.md` | Provides detailed instructions and information for Assignment 3. |
| **Assignment4_MIPSWithCache/** | Directory for the MIPS simulator with cache. |
| ├── `mips_cache_pipeline.c` | The MIPS pipeline implementation including cache functionality. |
| ├── `input.bin` | The binary input file for testing the processor. |
| └── `README.md` | Provides detailed instructions and information for Assignment 4. |
| ... | ... |


---

## Course Information

- **Course**: Computer Architecture (컴퓨터구조)
- **Semester**: Spring 2024 (2nd Year, 1st Semester)
- **University**: Dankook University
- **Department**: Mobile System Engineering









