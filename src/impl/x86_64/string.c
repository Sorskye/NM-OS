#include "types.h"

Char *IntToString(Int value, Char *str, Byte base)
{
    // Check for NULL buffer
    if (str == NULL) {
        return NULL;
    }
    
    // Validate base
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }
    
    // Pointers for character manipulation
    Char *rc = str;
    Char *ptr = str;
    Char *low;

    // Handle negative numbers for base 10
    unsigned int abs_value = (unsigned int)value;
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        abs_value = (unsigned int)(-value);  // Convert to positive for conversion
    } else {
        abs_value = (unsigned int)value;  // For positive values, set abs_value
    }

    // Mark the start of the number portion
    low = ptr;

    // Conversion loop: process abs_value with given base
    do {
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[abs_value % base];
        abs_value /= base;
    } while (abs_value > 0);

    // Terminate the string
    *ptr = '\0';

    // Reverse the number part to get the correct order
    for (--ptr; low < ptr; ++low, --ptr) {
        char tmp = *low;
        *low = *ptr;
        *ptr = tmp;
    }

    return rc;
}
