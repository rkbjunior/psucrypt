#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include "helpers.h"
#include "fileio.h"

using namespace std;

void GetPlainTextFromFile(vector<char>* plainTextBuffer, string fileName) {
    ifstream plaintextFile(fileName, ios::in | ios::binary | std::ios::ate);
    streamsize psize = plaintextFile.tellg();

    while (psize % 8 != 0) {

        psize++;
    }

    plainTextBuffer->resize(psize);

    plaintextFile.seekg(0, std::ios::beg);

    plaintextFile.read(plainTextBuffer->data(), psize);

}

void GetCipherTextFromFile(vector<uint64_t>* cipherTextBuffer, string fileName) {
    ifstream ciphertextFile;
    ciphertextFile.open(fileName, ios::in | ios::binary);

    uint64_t result = 0;

    while (!ciphertextFile.eof()) {
        ciphertextFile >> std::hex;
        ciphertextFile >> result;

        cipherTextBuffer->push_back(result);
    }
    cipherTextBuffer->pop_back();
}

void WriteToFile(uint64_t ciphertext, const char* fileName) {
    ofstream outputFile;

    outputFile.open(fileName, ios::out | ios::binary | ios::app);

    if (fileName == "ciphertext.txt") {
        outputFile << std::hex << ciphertext << std::dec << " ";
    }
    else {
        string ptext = Convert64ToString(ciphertext);

        outputFile << ptext;
    }

}

uint64_t GetKeyFromFile(string fileName) {
    ifstream keyFile;
    keyFile.open(fileName, ios::in | ios::binary);

    uint64_t result = 0;

    keyFile >> std::hex;
    keyFile >> result;

    return result;
}