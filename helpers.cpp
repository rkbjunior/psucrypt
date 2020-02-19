#include <cstdint>
#include <string>

using namespace std;

int getSkipJackIndex(uint8_t value) {
    unsigned int row = (value >> 4) & 0xF;
    unsigned int col = (value) & 0xF;

    int index = 16 * row;
    index += col;

    return index;
}

uint64_t leftRotate(uint64_t v, int n) {
    n = n & 63U;
    if (n) {
        v = (v << n) | (v >> (64 - n));
    }

    return v;
}