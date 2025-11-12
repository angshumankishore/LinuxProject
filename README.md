# 🧩 Dynamic Library Dependency Mapper (C Version)

A lightweight **C program** that analyzes ELF binaries to list their **shared library dependencies** by directly reading ELF headers — no external tools required.

This tool is similar to running `ldd`, but is **safer**, since it does **not execute** the target binary. Instead, it parses ELF metadata (using `<elf.h>`) to extract the required shared libraries (`DT_NEEDED` entries) and optionally resolves their paths.

---

## 🚀 Features

- Reads ELF binaries directly (no code execution)
- Extracts all shared library dependencies (`DT_NEEDED`)
- Works on both executables and shared objects (`.so` files)
- Supports both 32-bit and 64-bit ELF formats (can be extended easily)
- Prints dependencies in a clean, tree-like structure
- Detects missing libraries

---

## 📦 Requirements

- GCC or Clang (for compilation)
- A Linux system with `<elf.h>` available (usually part of `glibc`)

---

## 🧠 How It Works

1. The program opens a binary file and maps it into memory using `mmap()`.
2. It verifies that the file starts with the ELF magic bytes (`0x7f E L F`).
3. It locates the **Program Header Table (PHT)**.
4. It finds the **PT_DYNAMIC** segment, which contains information about dynamic linking.
5. It reads `DT_NEEDED` entries to identify all required shared libraries.
6. It prints the dependency names, optionally resolving their absolute paths.

---

## ⚙️ Usage

### 1️⃣ Compile

```bash
gcc -o dep_mapper dep_mapper.c

