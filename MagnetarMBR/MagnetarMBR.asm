bits 16 ; Bootloader using 16 bits.
org 0x7c00 ; BIOS magic number.

;Clear screen + set position.
    xor al, al
    xor bx, bx
    mov cx, 0x01
	
repeat: ;repeat entry point.

    ; Print color and txt.
    mov ah, 0x09
    int 0x10
    mov ah, 0x0e
    int 0x10
    
    ; Incrementing some register to generate random character and color (i little forgot about that).
    inc al
    inc bl
    inc cl
	
    inc ah
    inc bh
    inc ch

    ; Jump again to repeat entry point (loops).
    jmp repeat
 
times 510-($-$$) db 0 ; Fill until reach 510b.
dw 0xAA55 ; 2b mbr signature.
