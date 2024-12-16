section .multiboot_header
align 8
header_start:
    ; Multiboot2 magic number
    dd 0xe85250d6         ; Multiboot2 header magic number
    ; Architecture (0 for i386)
    dd 0
    ; Header length
    dd header_end - header_start
    ; Checksum
    dd -(0xe85250d6 + 0 + (header_end - header_start))

    ; End tag with proper padding
    align 8
    dw 0                  ; End tag type
    dw 0                  ; Reserved
    dd 8                  ; Size of end tag (8 bytes)
header_end:
