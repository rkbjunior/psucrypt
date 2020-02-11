#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef>
#include <cstdint>

using namespace std;

//untested, need f function first
uint16_t g_permutation(vector<uint8_t> w, int round, const vector<uint8_t> *subkeys) {
    const char ftable[] = { 0xa3,0xd7,0x09,0x83,0xf8,0x48,0xf6,0xf4,0xb3,
        0x21,0x15,0x78,0x99,0xb1,0xaf,0xf9,0xe7,0x2d,0x4d,0x8a,0xce,
        0x4c,0xca,0x2e,0x52,0x95,0xd9,0x1e,0x4e,0x38,0x44,0x28,0x0a,
        0xdf,0x02,0xa0,0x17,0xf1,0x60,0x68,0x12,0xb7,0x7a,0xc3,0xe9,
        0xfa,0x3d,0x53,0x96,0x84,0x6b,0xba,0xf2,0x63,0x9a,0x19,0x7c,
        0xae,0xe5,0xf5,0xf7,0x16,0x6a,0xa2,0x39,0xb6,0x7b,0x0f,0xc1,
        0x93,0x81,0x1b,0xee,0xb4,0x1a,0xea,0xd0,0x91,0x2f,0xb8,0x55,
        0xb9,0xda,0x85,0x3f,0x41,0xbf,0xe0,0x5a,0x58,0x80,0x5f,0x66,
        0x0b,0xd8,0x90,0x35,0xd5,0xc0,0xa7,0x33,0x06,0x65,0x69,0x45,
        0x00,0x94,0x56,0x6d,0x98,0x9b,0x76,0x97,0xfc,0xb2,0xc2,0xb0,
        0xfe,0xdb,0x20,0xe1,0xeb,0xd6,0xe4,0xdd,0x47,0x4a,0x1d,0x42,
        0xed,0x9e,0x6e,0x49,0x3c,0xcd,0x43,0x27,0xd2,0x07,0xd4,0xde,
        0xc7,0x67,0x18,0x89,0xcb,0x30,0x1f,0x8d,0xc6,0x8f,0xaa,0xc8,
        0x74,0xdc,0xc9,0x5d,0x5c,0x31,0xa4,0x70,0x88,0x61,0x2c,0x9f,
        0x0d,0x2b,0x87,0x50,0x82,0x54,0x64,0x26,0x7d,0x03,0x40,0x34,
        0x4b,0x1c,0x73,0xd1,0xc4,0xfd,0x3b,0xcc,0xfb,0x7f,0xab,0xe6,
        0x3e,0x5b,0xa5,0xad,0x04,0x23,0x9c,0x14,0x51,0x22,0xf0,0x29,
        0x79,0x71,0x7e,0xff,0x8c,0x0e,0xe2,0x0c,0xef,0xbc,0x72,0x75,
        0x6f,0x37,0xa1,0xec,0xd3,0x8e,0x62,0x8b,0x86,0x10,0xe8,0x08,
        0x77,0x11,0xbe,0x92,0x4f,0x24,0xc5,0x32,0x36,0x9d,0xcf,0xf3,
        0xa6,0xbb,0xac,0x5e,0x6c,0xa9,0x13,0x57,0x25,0xb5,0xe3,0xbd,
        0xa8,0x3a,0x01,0x05,0x59,0x2a,0x46 };

    const uint8_t g1 = w.at(1);
    const uint8_t g2 = w.at(0);

    uint8_t g3 = ftable[g2 ^ subkeys->at(4 * round)] ^ g1;
    uint8_t g4 = ftable[g3 ^ subkeys->at(4 * round + 1)] ^ g2;
    uint8_t g5 = ftable[g4 ^ subkeys->at(4 * round + 2)] ^ g3;
    uint8_t g6 = ftable[g5 ^ subkeys->at(4 * round + 3)] ^ g4;

    uint16_t g56 = ((uint16_t)g6 << 8) | g5;

    return g56;

}
//Rotates a 64bit block right by 1 bit
uint64_t rotate(uint64_t v, int n) {
    n = n & 63U;
    if (n) {
        v = (v >> n) | (v << (64 - n));
    }

    return v;
}

//Generates all the round subkeys in one go
void encrypt_ksched(int x, uint64_t * key, vector<uint8_t> * subkeys) {
    for(x; x < 192; x++) {
        //step 1   6067818064129581665
        //step 191 6067818064129581665  AWESOME!

        *key = rotate(*key, 1);

        //break the key into 8 bit parts 
        vector<uint8_t> k;

        for (int i = 0; i < 8; i++) {
            k.push_back((*key >> (i * 8)) & 0x00000000000000FF);
        }

        int keyIndex = x % 8;

        subkeys->push_back(k[keyIndex]);
    }
}

//Reads the plaintext into a file
//Returns a vector of chars of the plaintext.
vector<char> GetPlainText() {
    ifstream plaintextFile("plaintext.txt", ios::in | ios::binary | std::ios::ate);
    streamsize psize = plaintextFile.tellg();

    plaintextFile.seekg(0, std::ios::beg);

    vector<char> plainTextBuffer(psize);

    plaintextFile.read(plainTextBuffer.data(), psize);

    return plainTextBuffer;
}

//Reads the key from a file
//returns a uint64_t representation of the key
uint64_t GetKey() {
    ifstream keyFile("key.txt", ios::in | ios::binary);
    char keyStream[sizeof(uint64_t)];

    keyFile.read(keyStream, sizeof(uint64_t));

    uint64_t result = 0;
    unsigned char value[sizeof(result)];
    memcpy(&result, keyStream, sizeof(result));

    return result;
}

//Key whitening step, takes the key and first 64bits of plaintext
//breaks them up into 16 bit words and then XOR's them together
//word by word k0 XOR p0, k1 XOR p1, k2 XOR p2, k3 XOR p3
//Takes the 64bit key and the first 64 bits of plaintext
//Returns a vector of 4 16 bit(words) XOR values
vector<uint16_t> WhitenKey(uint64_t key, uint64_t plaintext) {
    vector<uint16_t> wKey;
    vector<uint16_t> keyWords;
    vector<uint16_t> plaintextWords;

    for (int i = 0; i < 4; i++) {
        keyWords.push_back((key >> (i * 16)) & 0x000000000000FFFF);
        plaintextWords.push_back((plaintext >> (i * 16)) & 0x000000000000FFFF);
        wKey.push_back(keyWords[i] ^ plaintextWords[i]);
    }

    return wKey;
}

//Copies the first 64 bits of the plaintext buffer
//into a uint64t variable
//Takes a vector<char> of plaintext
//Returns a uint64_t representation of the the plaintext.
uint64_t GetBlock(vector<char> * plaintextBuffer) {
    char pStream[sizeof(uint64_t)];
    copy_n(plaintextBuffer->begin(), 8, pStream);

    uint64_t result = 0;
    memcpy(&result, pStream, sizeof(result));
    
    return result;
}

int main()
{
    vector<char> plaintextBuffer = GetPlainText();
    uint64_t key = GetKey();
    uint64_t plaintextBlock = GetBlock(&plaintextBuffer);

    plaintextBuffer.erase(plaintextBuffer.begin(), plaintextBuffer.begin() + 8);

    vector<uint16_t> wKey = WhitenKey(key, plaintextBlock);

    int round = 0;
    vector<uint8_t> subkeys;

    encrypt_ksched(round, &key, &subkeys);

    printf("hello from PSU_CRYPT!\n");
    return 0;
}



