#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include "psu_crypt.h"

using namespace std;

vector<char> GetPlainTextFromFile() {
    ifstream plaintextFile("plaintext.txt", ios::in | ios::binary | std::ios::ate);
    streamsize psize = plaintextFile.tellg();

    plaintextFile.seekg(0, std::ios::beg);

    vector<char> plainTextBuffer(psize);

    plaintextFile.read(plainTextBuffer.data(), psize);

    return plainTextBuffer;
}

uint64_t GetKeyFromFile() {
    ifstream keyFile;
    keyFile.open("key2.txt", ios::in | ios::binary);

    uint64_t result = 0;

    keyFile >> std::hex;
    keyFile >> result;


    return result;
}

uint64_t GetBlockOfPlainText(vector<char> * plaintextBuffer) {
    char pStream[sizeof(uint64_t)];
    std::reverse(plaintextBuffer->begin(), plaintextBuffer->end());
    copy_n(plaintextBuffer->begin(), 8, pStream);

    uint64_t result = 0;
    memcpy(&result, pStream, sizeof(result));
    
    return result;
}

int main()
{
    vector<char> plaintextBuffer = GetPlainTextFromFile();
    uint64_t key = GetKeyFromFile();
    uint64_t plaintextBlock = GetBlockOfPlainText(&plaintextBuffer);
    uint64_t cipherText;
    uint64_t plainText;

    plaintextBuffer.erase(plaintextBuffer.begin(), plaintextBuffer.begin() + 8);

    plaintextBlock = 0x7365637572697479;
    
    cipherText = encrypt(&key, &plaintextBlock);
    //plainText = decrypt(&key, &cipherText);


    cout << "Good Bye from PSU_CRYPT!" << endl;
    return 0;
}




