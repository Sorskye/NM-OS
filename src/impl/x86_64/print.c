#include "print.h"
#include "string.h"
#include <stdint.h>

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(size_t row) {
    struct Char empty = { ' ', color };
    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        clear_row(i);
    }
    col = 0;
    row = 0;
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground | (background << 4);
}

void print_newline() {
    col = 0;
    if (row < NUM_ROWS - 1) {
        row++;
    } else {
        for (size_t r = 1; r < NUM_ROWS; r++) {
            for (size_t c = 0; c < NUM_COLS; c++) {
                buffer[c + NUM_COLS * (r - 1)] = buffer[c + NUM_COLS * r];
            }
        }
        clear_row(NUM_ROWS - 1);
    }
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }
    if (col >= NUM_COLS) {
        print_newline();
    }
    buffer[col + NUM_COLS * row] = (struct Char) { (uint8_t) character, color };
    col++;
}

void print_str(char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}

void print_int(int num) {
    char buffer[12];
    int i = 10;

    buffer[11] = '\0';
    if (num == 0) {
        buffer[10] = '0';
        print_str(&buffer[10]);
        return;
    }

    int is_negative = num < 0;
    if (is_negative) num = -num;

    while (num && i) {
        buffer[i--] = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative) buffer[i--] = '-';
    print_str(&buffer[i + 1]);
}

void print_hex(uint64_t num) {
    const char* hex_chars = "0123456789ABCDEF";
    char buffer[17];
    buffer[16] = '\0';
    for (int i = 15; i >= 0; i--) {
        buffer[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    print_str("0x");
    print_str(buffer);
}

void print_stack_trace() {
    uintptr_t *stack_ptr;
    asm volatile ("mov %%rsp, %0" : "=r" (stack_ptr)); // Get current stack pointer

    print_str("\nStack Trace:\n");
    for (int i = 0; i < 10; i++) {
        print_str("0x");
        print_int((int)(uintptr_t)stack_ptr[i]);
        print_str("\n");
    }
}

void print_cpu_registers() {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, rip;
    asm volatile ("mov %%rax, %0" : "=r" (rax));
    asm volatile ("mov %%rbx, %0" : "=r" (rbx));
    asm volatile ("mov %%rcx, %0" : "=r" (rcx));
    asm volatile ("mov %%rdx, %0" : "=r" (rdx));
    asm volatile ("mov %%rsi, %0" : "=r" (rsi));
    asm volatile ("mov %%rdi, %0" : "=r" (rdi));
    asm volatile ("mov %%rbp, %0" : "=r" (rbp));
    asm volatile ("mov %%rsp, %0" : "=r" (rsp));
    asm volatile ("lea (%%rip), %0" : "=r" (rip));

    print_str("CPU Registers:\n");
    print_str("RAX: "); print_hex(rax); print_newline();
    print_str("RBX: "); print_hex(rbx); print_newline();
    print_str("RCX: "); print_hex(rcx); print_newline();
    print_str("RDX: "); print_hex(rdx); print_newline();
    print_str("RSI: "); print_hex(rsi); print_newline();
    print_str("RDI: "); print_hex(rdi); print_newline();
    print_str("RBP: "); print_hex(rbp); print_newline();
    print_str("RSP: "); print_hex(rsp); print_newline();
    print_str("RIP: "); print_hex(rip); print_newline();
}

void kernel_panic(const char* message) {
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_RED);
    print_clear();
    print_newline();

    print_str("The NM-OS kernel has run into an unrecoverable issue.\n");
    print_str("Details of the issue are provided below.\n");
    print_newline();

    print_str("KERNEL PANIC! FAULT_ID >> ");
    print_str(message);
    print_newline();
    print_str("Please reboot the system.\n");

    print_cpu_registers();  // Print all CPU register values
    print_stack_trace();    // Print the stack trace

    while (1) {
        asm("hlt");
    }
}
