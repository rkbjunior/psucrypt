#ifndef FILEIO_H
#define FILEIO_H

void GetPlainTextFromFile(vector<char>* plainTextBuffer, string fileName);

void GetCipherTextFromFile(vector<uint64_t>* cipherTextBuffer, string fileName);

void WriteToFile(uint64_t ciphertext, const char* fileName);

uint64_t GetKeyFromFile(string fileName);

#endif