# Embed Kit: Embedded C Utility Library

**Author:** Yogesh Vanjari  
**GitHub:** [github.com/Mr-Yogesh01](https://github.com/Mr-Yogesh01)  
**Email:** vanjariyogesh531@gmail.com  

## Module Overview
* **Ring Buffer (`ringbuf.c`):** A lightweight, zero-allocation FIFO circular buffer designed for deterministic, interrupt-safe operations (e.g., UART RX queues) with fast bitwise index wrap-around.

## Build Instructions

### Prerequisites
* GCC Compiler (`gcc`) supporting C99 standard.

### Compilation
Compile cleanly with strict warnings:
```bash
gcc -Wall -std=c99 ringbuf.c -o ringbuf
