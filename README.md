# Magnetar
![Views](https://komarev.com/ghpvc/?username=Nathantor&label=Repository+views&color=blue)
<br /> **Warning!, This repository is Malware!!!, I am not responsible for any damages caused by it!**
<br /> **Also dont destroy other persons Computer!, Its crime, I am not responsible for what you do!**

# Rewrite version
<br /> Updates:
<br /> 1. Move the language from C to C++.
<br /> 2. Adding and fixing some GDI payloads.
<BR /> 3. Change the rng (Random numbers generator) from Splitmix32 to Mulberry32 (I little trust to Mulberry32 since passes gjrand's 13 tests with no failures).

# Payloads
<br /> 1. Messagebox warnings.
<br /> 2. Overwrite first sector and several sectors starting 1024 bytes of `\\\\.\\PhysicalDrive0`.
<br /> 3. Overwrite 10 sectors of `\\\\.\\C:`.
<br /> 4. Mark process as critical.
<br /> 5. Terminate some processes and suspend explorer.exe (to avoid user refresh or open program).
<br /> 6. Restrict some applications.
<br /> 7. Launch processes to delete entire System32.
<br /> 8. GDI messes and play noises sounds.
<br /> 9. Crashing the system by calling emergency shutdown using `NtShutdownSystem` or BSOD by terminating critical process.
<br /> 10. MBR.

# Download and Source code
<br /> Download: Deleted (for safety reasons).
<br /> Source code: See above.

# Requirements to compile
<br /> 1. Visual Studio 2013 Professional.
<br /> 2. NASM (cmd: `nasm -f bin MagnetarMBR.asm -o MagnetarMBR.bin`).
<br /> 3. HxD Hex Editor (Use Export function, and then select C).
<br /> 4. qemu (For testing MBR).
<br /> 5. VM (For testing malware).
<br /> Good luck!

# About Magnetar name
<br /> [Read Here](https://en.m.wikipedia.org/wiki/Magnetar)
