# [Magnetar](https://en.m.wikipedia.org/wiki/Magnetar)
[![Views](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FNathantor%2FMagnetar&count_bg=%2300FFFF&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=Views&edge_flat=false)](https://hits.seeyoufarm.com)
<br /> **Warning!, This repository is Malware!!!, I am not responsible for any damages caused by it!**
<br /> **Also dont destroy other persons Computer!, Its crime, I am not responsible for what you do!**

# Rewrite version
<br /> Updates:
<br /> 1. Move the language from C to C++ (idk why i do it).
<br /> 2. Adding and fixing some GDI payloads.
<BR /> 3. Change the prng from Splitmix32 to Mulberry32 (I little trust to Mulberry32 since passes gjrand's 13 tests with no failures).

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
