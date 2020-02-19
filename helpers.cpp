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

//https://stackoverflow.com/questions/3790613/how-to-convert-a-string-of-hex-values-to-a-string
unsigned char hexval(unsigned char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else abort();
}

//https://stackoverflow.com/questions/3790613/how-to-convert-a-string-of-hex-values-to-a-string
void hex2ascii(const string& in, string& out)
{
    out.clear();
    out.reserve(in.length() / 2);
    for (string::const_iterator p = in.begin(); p != in.end(); p++)
    {
        unsigned char c = hexval(*p);
        p++;
        if (p == in.end()) break; // incomplete last digit - should report error
        c = (c << 4) + hexval(*p); // + takes precedence over <<
        out.push_back(c);
    }
}