bits 16
org 0x7C00

    xor al, al
    xor bx, bx
    mov cx, 0x01
	
    rainbow:
    mov ah, 0x09
    int 10h
    mov ah, 0x0e
    int 10h
	
	inc al
	inc bl
	inc cl
	
	inc ah
	inc bh
	inc ch

    jmp rainbow
 
times 510-($-$$) db 0
dw 0xAA55
