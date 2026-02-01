# Kernel-Level Night Mode Utility (Linux)

![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Linux_Kernel-FCC624?style=for-the-badge&logo=linux&logoColor=black)


> **A zero-dependency display utility for direct framebuffer manipulation in ultra-low resource environments.**

---

## 📖 Overview
This project is a lightweight C utility designed to implement "Night Mode" (Blue Light Filtering) by interacting directly with the Linux Kernel Framebuffer (`/dev/fb0`).

Unlike standard tools (Redshift, GNOME Night Light) which rely on heavy display server composers (X11/Wayland), this utility **bypasses the display server entirely** to manipulate raw pixel data in video memory.

**Target Hardware:** Legacy systems with extreme resource constraints.
* **Tested Device:** LG RD400 Laptop
* **Constraint:** 512MB RAM, Single Core CPU Intel Celeron M
* **Goal:** <2MB Memory Footprint, Zero Latency, O(1) Lookup complexity per pixel.

---

## Technical Architecture

This tool operates in **User Space** but acts on **Kernel Interfaces**. It treats the screen as a raw file, mapping video memory directly into the program's address space.


Key Mechanisms
ioctl (Device Control): Used to query the driver for fb_var_screeninfo (Resolution, BPP) and fb_fix_screeninfo (Memory Length, Stride/Pitch).

mmap (Zero-Copy Access): Instead of using write() syscalls which incur context-switching overhead, we map the physical framebuffer address 0xB8000... directly to a user-space pointer uint8_t *fbp.

Stride Management: Manually handles memory padding (the difference between width * 4 and line_length) to prevent image skewing, emulating 2D strided memory access patterns common in CUDA/HPC kernels.

## Optimization Methodology
A core component of this project is comparing naive implementations vs. systems-optimized approaches.

1. Integer Arithmetic vs. Floating Point
On the target Celeron M processor, the Floating Point Unit (FPU) is significantly slower than the Integer ALU.

Naive Approach: pixel = pixel * 0.75f; (Requires FPU, high latency)

Optimized Approach: pixel = (pixel * 192) >> 8; (Integer multiplication + Bit shift)

2. Compile-Time Lookup Tables (LUTs)
To further reduce CPU cycles inside the O(N) pixel loop (where N = 2,073,600 pixels for 1080p), I implemented pre-computed arrays.

Concept: Instead of calculating color values for every pixel, the program generates a 256-byte array at startup.

Result: The heavy multiplication logic is removed from the hot path. The loop becomes a simple memory fetch: output = LUT[input].

## Why "From Scratch"?
Modern Linux desktop environments assume hardware acceleration and ample RAM. On legacy hardware (512MB RAM), standard solutions fail:

Bloat: Tools like Redshift require Gtk/Python runtimes which consume ~50-100MB RAM.

Latency: Compositors introduce input lag on single-core CPUs.


This utility is a study in Systems Programming under constraints, proving that useful graphical effects can be achieved with pure C and kernel interfaces.

 Project Roadmap & Status
This repository is currently under active development.

##  Installation & Usage (Coming Soon)
Since the codebase is currently being finalized, these instructions are for the upcoming release.

Prerequisites
Linux Kernel (any version with fbdev enabled)

GCC Compiler

Root privileges (required to access /dev/fb0)

## Build
```bash
gcc -o nightmode main.c -O2
```
Run
```bash
sudo ./nightmode 100
```
