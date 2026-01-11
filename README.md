# Kernel-Level Night Mode Utility (Linux)

![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Linux_Kernel-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Status](https://img.shields.io/badge/Status-Active_Development-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

> **A zero-dependency display utility for direct framebuffer manipulation in ultra-low resource environments.**

---

## 📖 Overview
This project is a lightweight C utility designed to implement "Night Mode" (Blue Light Filtering) by interacting directly with the Linux Kernel Framebuffer (`/dev/fb0`).

Unlike standard tools (Redshift, GNOME Night Light) which rely on heavy display server composers (X11/Wayland), this utility **bypasses the display server entirely** to manipulate raw pixel data in video memory.

**Target Hardware:** Legacy systems with extreme resource constraints.
* **Tested Device:** LG RD400 Laptop
* **Constraint:** 512MB RAM, Single Core CPU
* **Goal:** <2MB Memory Footprint, Zero Latency

---

## ⚙️ Technical Architecture

This tool operates in **User Space** but acts on **Kernel Interfaces**. It treats the screen as a raw file, mapping video memory directly into the program's address space.


Key Mechanisms
ioctl System Calls: Retrieves variable screen info (vinfo) and fixed screen info (finfo) to understand resolution and bit depth (16-bit vs 32-bit).

mmap (Memory Mapping): Maps the framebuffer device file into the application's memory. This allows the program to treat the screen like a standard array of integers.

Bitwise Color Shifting:

Reads the pixel at offset x.

Deconstructs RGBA channels using bitmasks.

Applies a temperature coefficient to the Blue/Green channels.

Writes the pixel back instantly.

🚀 Why "From Scratch"?
Modern Linux desktop environments assume hardware acceleration and ample RAM. On legacy hardware (512MB RAM), standard solutions fail:

Bloat: Tools like Redshift require Gtk/Python runtimes which consume ~50-100MB RAM.

Latency: Compositors introduce input lag on single-core CPUs.

Dependency Hell: Older distros (e.g., antiX) may lack Wayland support.

This utility is a study in Systems Programming under constraints, proving that useful graphical effects can be achieved with pure C and kernel interfaces.

🛠 Project Roadmap & Status
This repository is currently under active development.

[x] Phase 1: Initialization

Open /dev/fb0

Retrieve screen geometry via ioctl

[x] Phase 2: Memory Management

Implement mmap for read/write access

Handle page alignment

[ ] Phase 3: Color Algorithms (In Progress)

[ ] Implement RGB extraction logic

[ ] Optimize temperature math (Move from float to integer arithmetic for performance)

[ ] Phase 4: CLI Interface

Add flags for temperature (e.g., --temp 3000K)

## 💻 Installation & Usage (Coming Soon)
Since the codebase is currently being finalized, these instructions are for the upcoming release.

Prerequisites
Linux Kernel (any version with fbdev enabled)

GCC Compiler

Root privileges (required to access /dev/fb0)

## Build

`gcc -o nightmode main.c -O2`

Run

`sudo ./nightmode`



### **One Final Tip for You:**
When you upload the code, include a file named `main.c` even if it is incomplete. Put the basic `open` and `ioctl` skeleton code in there.

Having a file named `main.c` makes the **Language: C** badge appear automatically on GitHub, which reinforces your profile as a C programmer.

### The Data Flow
```mermaid
graph LR
    A[User Space App] -->|ioctl| B(Get Screen Info)
    A -->|mmap| C(Video Memory / VRAM)
    C -->|Direct Write| D[Display Output]
    style A fill:#f9f,stroke:#333,stroke-width:2px
    style C fill:#ccf,stroke:#333,stroke-width:2px
