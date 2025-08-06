# Assignment 4: Pipelined MIPS with Cache Simulation

This project extends the pipelined MIPS emulator by adding cache simulation to model memory latency.

## Features

- Configurable cache (block size, associativity, size)
- LRU replacement algorithm
- Write-back / write-through policies
- Cache hit/miss and AMAT calculation

## Cache Details

- Default line size: 64 bytes
- Cache size: 64~256 bytes
- Associativity: direct-mapped, 2-way, 4-way, etc.
- Memory access latency: 1000 cycles, cache: 1 cycle

## Output Metrics

- Cache hit/miss counts (cold/conflict)
- Execution cycle count
- AMAT (Average Memory Access Time)

## Execution

- Compile: `gcc -o mips_cache mips_cache_pipeline.c`
- Run: `./mips_cache input.bin`
