#include <fstream>
#include <vector>
#include <cstdint>
#include "psu_crypt.h"

using namespace std;

vector<char> GetPlainText() {
    ifstream plaintextFile("plaintext.txt", ios::in | ios::binary | std::ios::ate);
    streamsize psize = plaintextFile.tellg();

    plaintextFile.seekg(0, std::ios::beg);

    vector<char> plainTextBuffer(psize);

    plaintextFile.read(plainTextBuffer.data(), psize);

    return plainTextBuffer;
}

uint64_t GetKey() {
    ifstream keyFile;
    keyFile.open("key2.txt", ios::in | ios::binary);

    uint64_t result = 0;

    keyFile >> std::hex;
    keyFile >> result;


    return result;
}

uint64_t GetBlock(vector<char> * plaintextBuffer) {
    char pStream[sizeof(uint64_t)];
    std::reverse(plaintextBuffer->begin(), plaintextBuffer->end());
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

    plaintextBlock = 0x7365637572697479;
    
    uint64_t wKey = WhitenKey(key, plaintextBlock);

    //Compute all the subkeys
    vector<uint8_t> subkeys;
    encrypt_ksched(&key, &subkeys);

    vector<uint16_t> result;

    uint16_t nextR3 = wKey & 0x000000000000FFFF;
    uint16_t nextR2 = (wKey >> 16) & 0x000000000000FFFF;
    uint16_t nextR1 = (wKey >> 32) & 0x000000000000FFFF;
    uint16_t nextR0 = (wKey >> 48) & 0x000000000000FFFF;

    int round = 0;
    for (int i = 0; i < 16; i++) {
        //save r0 and r1 so we can use them in the next round
        uint16_t tempR0 = nextR0;
        uint16_t tempR1 = nextR1;

        result = f(nextR0, nextR1, &round, &subkeys);
        nextR0 = nextR2 ^ result.at(0);
        nextR1 = nextR3 ^ result.at(1);
        nextR2 = tempR0;
        nextR3 = tempR1;

        round++;
    }

    //undo the swap
    uint64_t unswap = ((uint64_t)nextR1 << 48) + ((uint64_t)nextR0 << 32) + ((uint64_t)nextR3 << 16) + nextR2;

    uint64_t cipherText = WhitenKey(unswap, key);


    printf("hello from PSU_CRYPT!\n");
    return 0;
}




