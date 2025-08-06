# Assignment 3: Pipelined MIPS Emulator

This emulator extends the single-cycle model by introducing a 5-stage pipeline (IF, ID, EX, MEM, WB).

## Features

- Pipeline latches to store intermediate states
- Forwarding and stall logic to resolve data hazards
- Basic control hazard handling (delayed slot or invalidation)
- PC stops at `0xFFFFFFFF`

## Performance Comparison

- Clock cycles for pipelined vs. single-cycle execution
- Instruction throughput improvements
- Optional: branch prediction, visualization

## Execution

- Compile: `gcc -o pipeline mips_pipeline.c`
- Run: `./pipeline input.bin`

## Output

- Changed architectural states per cycle
- Final register values
- Instruction statistics (ALU, mem, branch, etc.)
