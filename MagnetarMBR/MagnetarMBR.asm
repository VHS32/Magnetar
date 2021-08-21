bits 16
org 0x7c00

    xor al, al
    xor bx, bx
    mov cx, 0x01
	
    rainbow:
    mov ah, 0x09
    int 0x10
    mov ah, 0x0e
    int 0x10

    inc al
    inc bl
    inc cl
	
    inc ah
    inc bh
    inc ch

    jmp rainbow
 
times 510-($-$$) db 0
dw 0xAA55
