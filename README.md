# SIMPLE Assembler & Emulator

This project implements a **two‑pass assembler** and a **SIMPLE machine emulator** for the extended SIMPLE instruction set. You can assemble `.asm` source files into binary object files and human‑readable listings, then run them under the emulator.

---

## Features

- **Two‑Pass Assembler (`asm.c`)**  
  - **Label resolution** (forward/backward references)  
  - **Error checking**: unknown mnemonics, duplicate/undefined labels, bad operands  
  - **Object file**: 32‑bit words, little‑endian binary starting at address 0  
  - **Listing file**: address, machine code, mnemonic/operand, and labels  

- **SIMPLE Emulator (`emu.c`)**  
  - **Registers**: A, B (stack), SP (stack pointer), PC (program counter)  
  - **Instruction set**:  
    `data`, `ldc`, `adc`, `ldl`, `stl`, `ldnl`, `stnl`, `add`, `sub`, `shl`, `shr`,  
    `adj`, `a2sp`, `sp2a`, `call`, `return`, `brz`, `brlz`, `br`, `HALT`, `SET`  
  - **Execution**: loads an object file, steps until `HALT`, dumps memory on demand  

---

## Getting Started

### Prerequisites

- **GCC** (ISO C89):  
  ```bash
  gcc --version
