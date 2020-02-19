#ifndef PSU_CRYPT_H
#define PSU_CRYPT_H

class psu_crypt {
public:
    uint64_t GetBlockOfPlainText(std::vector<char>* plainTextBuffer);

    uint16_t g(uint16_t w, int* round, const std::vector<uint8_t>* subkeys);

    std::vector<uint16_t> f(uint16_t r0, uint16_t r1, int* round, const std::vector<uint8_t>* subkeys);

    void encrypt_ksched(uint64_t* key, std::vector<uint8_t>* subkeys);

    uint64_t WhitenKey(uint64_t key, uint64_t plaintext);

    uint64_t encrypt_decrypt(uint64_t* key, uint64_t* plaintext, bool encrypt);

};
#endif