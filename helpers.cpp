#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include "helpers.h"

using namespace std;

/*
Calculates the row and column indexes for the skip jack table.

Param: value - the 8 bit value that represents the index to the skip jack table.
The upper half of the value is row and the lower half of the value is the column.

Returns: An integer of the index for the skip jack lookup.
*/
int helpers::getSkipJackIndex(uint8_t value) {
    unsigned int row = (value >> 4) & 0xF;
    unsigned int col = (value) & 0xF;

    int index = 16 * row;
    index += col;

    return index;
}

/*
Left ciruclar rotates a uint64_t by n bits

Param: v - The uint64_t to rotate
Param: n - The number of bits to rotate

Returns: The rotated uint64_t value
*/
uint64_t helpers::leftRotate(uint64_t v, int n) {
    n = n & 63U;
    if (n) {
        v = (v << n) | (v >> (64 - n));
    }

    return v;
}

/*
Converts a uint64_t into an ascii equivalent string

Param: value -  a uint64_t value to be converted

Returns: An ascii representation of a uint64_t
*/
string helpers::Convert64ToString(uint64_t value) {
    string result;

    for (int i = 7; i >= 0; i--) {
        result.push_back(value >> (i * 8) & 0x0000000000FF);
    }

    return result;
}