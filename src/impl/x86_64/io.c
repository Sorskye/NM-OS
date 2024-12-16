#include "types.h"
#include "lock.h"
#include "string.h"
#include "io.h"
#include "print.h"

UByte InByte(UShort port)
{
    UByte result;
    asm volatile("inb %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
}

void OutByte(UShort port, UByte data)
{
    asm volatile("outb %0, %1" ::"a"(data), "Nd"(port));
}

// ---------------- SERIAL ---------------- //

#define COM1 0x3f8

Bool InitSerial()
{
    // Disable interrupts, set baud rate divisor, and line control
    OutByte(COM1 + 1, 0x00); // Disable all interrupts
    OutByte(COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
    OutByte(COM1 + 0, 0x03); // Set divisor to 3 (38400 baud)
    OutByte(COM1 + 1, 0x00);

    // Set 8 bits, no parity, 1 stop bit, and enable FIFO
    OutByte(COM1 + 3, 0x03); // Line control
    OutByte(COM1 + 2, 0xC7); // Enable FIFO, clear, with 14-byte threshold
    OutByte(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set

    // Set loopback mode and test the serial chip
    OutByte(COM1 + 4, 0x1E);
    OutByte(COM1 + 0, 0xAE);

    // Check for serial chip error
    UByte recv = InByte(COM1 + 0);
    if (recv != 0xAE)
    {
        print_str("Serial test failed. Received byte: ");
        Char buf[4];
        print_str(IntToString((Int)recv, buf, 16));
        return False;
    }

    // Set normal mode
    OutByte(COM1 + 4, 0x0F);
    return True;
}


static Byte _IsTransmitEmpty()
{
    return InByte(COM1 + 5) & 0x20;
}

void SerialChar(char ch)
{
    WAIT_UNTIL(_IsTransmitEmpty() != 0);
    OutByte(COM1, ch);
}

void SerialPrint(Char *str)
{
    WAIT_UNTIL(_IsTransmitEmpty() != 0);
    if (!str) return;
    for (Size i = 0; str[i] != '\0'; i++) {
        SerialChar(str[i]);
    }
}
void SerialPrintln(Char *str)
{
    WAIT_UNTIL(_IsTransmitEmpty() != 0);
    for (Size i = 0; 1; i++)
    if (!str) return;
    for (Size i = 0; str[i] != '\0'; i++) {
        SerialChar(str[i]);
    }

}