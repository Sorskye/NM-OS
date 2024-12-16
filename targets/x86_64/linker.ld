ENTRY(start)

SECTIONS
{
    . = 1M;  /* Start at 1MB to ensure space for the header and avoid conflicts with the bootloader */

    /* Place the multiboot header at the very start of the kernel */
    .boot : 
    {
        KEEP(*(.multiboot_header))  /* Keep the multiboot header in the final binary */
    }

    /* The text section for the kernel */
    .text : 
    {
        *(.text)  /* Include all the code from .text sections */
    }

    /* Additional sections can go here (e.g., data, bss, etc.) */
}
