bits 16 ; Bootloader using 16 bits.
org 0x7c00 ; BIOS magic number.

    ; Preparing.
    xor al, al
    xor bx, bx
    mov cx, 0x01
	
repeat: ;repeat entry point.

    ; Print character.
    mov ah, 0x09
    int 0x10
    mov ah, 0x0e
    int 0x10
    
    ; Rainbow and random.
    inc al
    inc bl
    inc cl
	
    inc ah
    inc bh
    inc ch

    ; Jump to repeat entry point (loops).
    jmp repeat
 
times 510-($-$$) db 0 ; Fill until reach 510b.
dw 0xaa55 ; 2b mbr signature.
