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

### The Data Flow
```mermaid
graph LR
    A[User Space App] -->|ioctl| B(Get Screen Info)
    A -->|mmap| C(Video Memory / VRAM)
    C -->|Direct Write| D[Display Output]
    style A fill:#f9f,stroke:#333,stroke-width:2px
    style C fill:#ccf,stroke:#333,stroke-width:2px
