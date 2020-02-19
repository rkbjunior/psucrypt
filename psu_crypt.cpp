#include <cstdint>
#include <vector>
#include <iomanip>
#include <iostream>
#include "helpers.h"

using namespace std;

uint16_t g(uint16_t w, int* round, const vector<uint8_t>* subkeys) {
    const int ftable[] = {
        0xa3,0xd7,0x09,0x83,0xf8,0x48,0xf6,0xf4,0xb3,0x21,0x15,0x78,0x99,0xb1,0xaf,0xf9,
        0xe7,0x2d,0x4d,0x8a,0xce,0x4c,0xca,0x2e,0x52,0x95,0xd9,0x1e,0x4e,0x38,0x44,0x28,
        0x0a,0xdf,0x02,0xa0,0x17,0xf1,0x60,0x68,0x12,0xb7,0x7a,0xc3,0xe9,0xfa,0x3d,0x53,
        0x96,0x84,0x6b,0xba,0xf2,0x63,0x9a,0x19,0x7c,0xae,0xe5,0xf5,0xf7,0x16,0x6a,0xa2,
        0x39,0xb6,0x7b,0x0f,0xc1,0x93,0x81,0x1b,0xee,0xb4,0x1a,0xea,0xd0,0x91,0x2f,0xb8,
        0x55,0xb9,0xda,0x85,0x3f,0x41,0xbf,0xe0,0x5a,0x58,0x80,0x5f,0x66,0x0b,0xd8,0x90,
        0x35,0xd5,0xc0,0xa7,0x33,0x06,0x65,0x69,0x45,0x00,0x94,0x56,0x6d,0x98,0x9b,0x76,
        0x97,0xfc,0xb2,0xc2,0xb0,0xfe,0xdb,0x20,0xe1,0xeb,0xd6,0xe4,0xdd,0x47,0x4a,0x1d,
        0x42,0xed,0x9e,0x6e,0x49,0x3c,0xcd,0x43,0x27,0xd2,0x07,0xd4,0xde,0xc7,0x67,0x18,
        0x89,0xcb,0x30,0x1f,0x8d,0xc6,0x8f,0xaa,0xc8,0x74,0xdc,0xc9,0x5d,0x5c,0x31,0xa4,
        0x70,0x88,0x61,0x2c,0x9f,0x0d,0x2b,0x87,0x50,0x82,0x54,0x64,0x26,0x7d,0x03,0x40,
        0x34,0x4b,0x1c,0x73,0xd1,0xc4,0xfd,0x3b,0xcc,0xfb,0x7f, 0xab, 0xe6,0x3e,0x5b,0xa5,
        0xad,0x04,0x23,0x9c,0x14,0x51,0x22,0xf0,0x29,0x79,0x71,0x7e,0xff,0x8c,0x0e,0xe2,
        0x0c,0xef,0xbc,0x72,0x75,0x6f,0x37,0xa1,0xec,0xd3,0x8e,0x62,0x8b,0x86,0x10,0xe8,
        0x08,0x77,0x11,0xbe,0x92,0x4f,0x24,0xc5,0x32,0x36,0x9d,0xcf,0xf3,0xa6,0xbb,0xac,
        0x5e,0x6c,0xa9,0x13,0x57,0x25,0xb5,0xe3,0xbd,0xa8,0x3a,0x01,0x05,0x59,0x2a,0x46
    };

    const uint8_t g1 = (w >> 8) & 0xFF;
    const uint8_t g2 = w & 0xFF;

    uint8_t g3 = ftable[getSkipJackIndex(g2 ^ subkeys->at(4 * *round))] ^ g1;
    uint8_t g4 = ftable[getSkipJackIndex(g3 ^ subkeys->at(4 * *round + 1))] ^ g2;
    uint8_t g5 = ftable[getSkipJackIndex(g4 ^ subkeys->at(4 * *round + 2))] ^ g3;
    uint8_t g6 = ftable[getSkipJackIndex(g5 ^ subkeys->at(4 * *round + 3))] ^ g4;

    uint16_t g56 = ((uint16_t)g5 << 8) | g6;

    *round = *round + 1;

    cout << "G Permutations: ";
    cout << "g1: " << std::hex << unsigned(g1) << std::dec;
    cout << " g2: " << std::hex << unsigned(g2) << std::dec;
    cout << " g3: " << std::hex << unsigned(g3) << std::dec; 
    cout << " g4: " << std::hex << unsigned(g4) << std::dec;
    cout << " g5: " << std::hex << unsigned(g5) << std::dec;
    cout << " g6: " << std::hex << unsigned(g6) << std::dec << endl;

    return g56;

}

vector<uint16_t> f(uint16_t r0, uint16_t r1, int* round, const vector<uint8_t>* subkeys) {

    //t0 = 33523  t1 = 63870
    uint16_t t0 = g(r0, round, subkeys);
    uint16_t t1 = g(r1, round, subkeys);

    cout << "t0: " << std::hex << unsigned(t0) << std::dec;
    cout << " t1: " << std::hex << unsigned(t1) << std::dec << endl;

    //uint8_t subKey0 = subkeys->at(4 * (*round));
    //uint8_t subKey1 = subkeys->at(4 * (*round) + 1);
    //uint8_t subKey2 = subkeys->at(4 * (*round) + 2);
    //uint8_t subKey3 = subkeys->at(4 * (*round) + 3);

    uint16_t key1 = (subkeys->at(4 * (*round)) << 8) | subkeys->at(4 * (*round) + 1);
    uint16_t key2 = (subkeys->at(4 * (*round) + 2) << 8) | subkeys->at(4 * (*round) + 3);

    uint16_t f0 = (t0 + 2 * t1 + key1) % 65536;
    uint16_t f1 = (2 * t0 + t1 + key2) % 65536;

    cout << "f0: " << std::hex << unsigned(f0) << std::dec;
    cout << " f1: " << std::hex << unsigned(f1) << std::dec << endl;

    vector<uint16_t> result;
    result.push_back(f0);
    result.push_back(f1);

    return result;
}

void encrypt_ksched(uint64_t* key, vector<uint8_t>* subkeys) {

    int round = 0;
    bool toggle = false;
    for (int i = 0; i < 192; i++) {

        uint32_t tempkey;
        uint8_t subKey;

        *key = leftRotate(*key, 1);

        if (toggle) {
            tempkey = *key >> 32 & 0x00000000FFFFFFFF;
        }
        else {
            tempkey = *key & 0x00000000FFFFFFFF;
        }

        int keyIndex = i % 4;

        subKey = tempkey >> (keyIndex * 8) & 0x00000000000000FF;

        subkeys->push_back(subKey);

        round++;

        if (round % 12 == 0) {
            toggle = !toggle;
        }
    }
}

uint64_t WhitenKey(uint64_t key, uint64_t plaintext) {

    uint64_t wKey = key ^ plaintext;
    return wKey;
}

uint64_t encrypt_decrypt(uint64_t * key, uint64_t * plaintextBlock, bool encrypt) {
    uint64_t wKey = WhitenKey(*key, *plaintextBlock);

    vector<uint8_t> subkeys;
    std::string mode = "ENCRYPTION";
    std::string blockType = "Plaintext: ";

    encrypt_ksched(key, &subkeys);

    if (!encrypt) {
        reverse(subkeys.begin(), subkeys.end());

        for (int i = 0; i < 192; (i += 12)) { 
            reverse(subkeys.begin() + i, subkeys.begin() + (i + 12));
        }
        mode = "DECRYPTION";
        blockType = "Ciphertext: ";
    }
    else {
        encrypt_ksched(key, &subkeys);
    }

    vector<uint16_t> result;

    uint16_t nextR3 = wKey & 0x000000000000FFFF;
    uint16_t nextR2 = (wKey >> 16) & 0x000000000000FFFF;
    uint16_t nextR1 = (wKey >> 32) & 0x000000000000FFFF;
    uint16_t nextR0 = (wKey >> 48) & 0x000000000000FFFF;

    cout << mode << endl;
    cout << blockType << std::hex << *plaintextBlock << std::dec << endl;
    cout << "Key = " << std::hex << *key << std::dec << endl;
    cout << "Whitened Key = " << std::hex << wKey << std::dec << endl << endl;

    int round = 0;
    for (int i = 0; i < 16; i++) {

        cout << "Beginning of round " << i << endl;
        cout << "Keys (hex): ";

        for (int j = 0; j < 12; j++) {
            cout << std::hex << unsigned(subkeys[j]) << std::dec << " ";
        }

        cout << endl;
        cout << "Keys (dec): ";

        for (int j = 0; j < 12; j++) {
            cout << unsigned(subkeys[j]) << " ";
        }

        cout << endl;

        //save r0 and r1 so we can use them in the next round
        uint16_t tempR0 = nextR0;
        uint16_t tempR1 = nextR1;

        result = f(nextR0, nextR1, &round, &subkeys);
        nextR0 = nextR2 ^ result.at(0);
        nextR1 = nextR3 ^ result.at(1);
        nextR2 = tempR0;
        nextR3 = tempR1;

        round++;

        cout << endl << endl;
    }

    uint16_t y0 = nextR2;
    uint16_t y1 = nextR3;
    uint16_t y2 = nextR0;
    uint16_t y3 = nextR1;

    uint64_t unswap = ((uint64_t)y0 << 48) + ((uint64_t)y1 << 32) + ((uint64_t)y2 << 16) + y3;

    //undo the swap
    //uint64_t unswap = ((uint64_t)nextR1 << 48) + ((uint64_t)nextR0 << 32) + ((uint64_t)nextR3 << 16) + nextR2;

    uint64_t block = WhitenKey(unswap, *key);

    if (blockType == "Ciphertext: ") {
        cout << "Plaintext: 0x" << std::hex << unsigned(block >> 32) << unsigned(block) << std::dec << endl;
    }
    else {
        cout << "Ciphertext: 0x" << std::hex << unsigned(block >> 32) << unsigned(block) << std::dec << endl;
    }

    return block;
}