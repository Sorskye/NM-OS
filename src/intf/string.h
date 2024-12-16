#pragma once
#include "types.h"

// Converts an integer value to a string representation based on the specified base.
// Supports bases from 2 to 36. If the base is invalid, returns an empty string.
Char *IntToString(Int value, Char *str, Byte base);
