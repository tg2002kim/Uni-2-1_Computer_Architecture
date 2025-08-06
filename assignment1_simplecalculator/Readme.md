# Assignment 1: Simple Register-Based Calculator

This project implements a simple virtual calculator using register operations. Instructions are provided in text format and parsed to execute arithmetic operations and control flows.

## Features

- 32 virtual registers (`r0` to `r31`)
- Instruction set: `add`, `sub`, `mul`, `div`, `mov`, `halt`
- Basic error handling and print of final register states

## Usage

1. Place instruction set in `input.txt`
2. Compile: `gcc -o calc main.c`
3. Run: `./calc input.txt`

## Purpose

To practice parsing and simulating instruction execution flow using register-based logic.
