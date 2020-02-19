#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include "psu_crypt.h"
#include "helpers.h"

using namespace std;

vector<char> GetPlainTextFromFile() {
    ifstream plaintextFile("plaintext.txt", ios::in | ios::binary | std::ios::ate);
    streamsize psize = plaintextFile.tellg();

    plaintextFile.seekg(0, std::ios::beg);

    vector<char> plainTextBuffer(psize);

    plaintextFile.read(plainTextBuffer.data(), psize);

    return plainTextBuffer;
}

void WriteToFile(uint64_t ciphertext) {
    ofstream outputFile;
    outputFile.open("ciphertext.txt");
    
    outputFile << ciphertext;

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
    //Read the files in this case the secret key is ab$ra85T
    vector<char> plaintextBuffer = GetPlainTextFromFile();
    uint64_t key = GetKeyFromFile();

    //Get the first 64 bit block of plaintext to encrypt/decrypt
    //Then remove those bits from the buffer so we dont read them again

    while (plaintextBuffer.size() > 0) {
        uint64_t plaintextBlock = GetBlockOfPlainText(&plaintextBuffer);
        plaintextBuffer.erase(plaintextBuffer.begin(), plaintextBuffer.begin() + 8);

        uint64_t cipherText = encrypt_decrypt(&key, &plaintextBlock, true);

        WriteToFile(cipherText);

    }


   // uint64_t plainText = encrypt_decrypt(&key, &cipherText, false);

    


    cout << "Good Bye from PSU_CRYPT!" << endl;
    return 0;
}




