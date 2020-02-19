#ifndef FILEIO_H
#define FILEIO_H

class fileio {
public:
    void WriteToFile(uint64_t ciphertext, const char* fileName);
    uint64_t GetKeyFromFile(std::string fileName);
    void GetPlainTextFromFile(std::vector<char>* plainTextBuffer, std::string fileName);
    void GetCipherTextFromFile(std::vector<uint64_t>* cipherTextBuffer, std::string fileName);
};
#endif