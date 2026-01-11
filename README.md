⚡ Overview
A lightweight, zero-dependency C utility designed to implement "Night Mode" (Blue Light Filtering) by interacting directly with the Linux Kernel Framebuffer (/dev/fb0).

Unlike standard tools (Redshift, GNOME Night Light) which rely on heavy display server composers (X11/Wayland), this utility bypasses the display server entirely to manipulate raw pixel data.

Target Hardware: Legacy systems with extreme resource constraints (Tested on: LG RD400, 512MB RAM).

🔧 Technical Architecture:

This tool operates in User Space but interacts directly with Kernel Space interfaces to modify the display output.

The Mechanism:
Device Access: Opens the framebuffer device file (/dev/fb0).

Screen Info Retrieval: Uses ioctl system calls to retrieve variable screen information (vinfo) and fixed screen information (finfo) (resolution, bit depth, offset).

Direct Memory Mapping: Uses mmap() to map the framebuffer device into the process's memory space, allowing direct read/write access to the pixel array.

Pixel Manipulation:

Iterates through the mapped memory buffer.

Performs bitwise operations on RGBA values to reduce the Blue channel intensity based on a calculated temperature curve.

Writes modified bytes back to the buffer instantly.

🚀 Why "From Scratch"? (The Problem)
Existing solutions were unsuitable for the target hardware environment:

Bloat: Standard tools require heavy dependencies (Gtk, Python runtimes, X11 extensions).

Latency: Compositors introduce input lag on single-core legacy CPUs.

Memory Footprint: The target device (512MB RAM) creates a hard constraint. This utility runs with <2MB memory footprint.

🛠 Implementation Details (In Progress)
Current State:

[x] Framebuffer device initialization.

[x] mmap implementation for read/write access.

[ ] Optimization of the color-shifting algorithm (moving from floating point to integer arithmetic for speed).

[ ] Handling different pixel formats (16-bit vs 32-bit depth).

Note: This repository is currently under active development as part of a Systems Programming study.
