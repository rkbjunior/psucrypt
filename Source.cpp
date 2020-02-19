#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include "psu_crypt.h"
#include "helpers.h"

using namespace std;

int LOGGING = 1;

string Convert64ToString(uint64_t value) {
    string result;

    for (int i = 7; i >= 0; i--) {
        result.push_back(value >> (i * 8) & 0x0000000000FF);
    }

    return result;
}

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

void WriteToFile(uint64_t ciphertext, const char * fileName) {
    ofstream outputFile;
    string str;
    uint64_t out;

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

uint64_t GetBlockOfPlainText(vector<char> * plaintextBuffer) {
    char pStream[sizeof(uint64_t)];
    std::reverse(plaintextBuffer->begin(), plaintextBuffer->begin() + 8);
    copy_n(plaintextBuffer->begin(), 8, pStream);

    //reverse(pStream, &pStream[strlen(pStream)]);

    uint64_t result = 0;
    memcpy(&result, pStream, sizeof(result));
    
    return result;
}

uint64_t GetBlockOfCipherText(vector<uint64_t>* ciphertextBuffer) {
    char pStream[sizeof(uint64_t)];
    //std::reverse(ciphertextBuffer->begin(), ciphertextBuffer->end());

    return ciphertextBuffer->at(0);
}

int main()
{
    //If the ciphertext.txt exists, delete it.
    if (LOGGING == 1 || LOGGING == 0)
        cout << "Cleaning up some stuff...\n\n";

    if (remove("ciphertext.txt") == 0) {
        if (LOGGING == 1 || LOGGING == 0)
            cout << "Deleted ciphertext.txt\n\n" ;
    }
    else {
        if (LOGGING == 1 || LOGGING == 0)
            cout << "ciphertext.txt was not found or has open handles\n\n";
    }

    //If the decrypted_plaintext.txt exists, delete it.
    if (remove("decrypted_plaintext.txt") == 0) {
        if (LOGGING == 1 || LOGGING == 0)
            cout << "Deleted decrypted_plaintext.txt\n\n";
    }
    else {
        if (LOGGING == 1 || LOGGING == 0)
            cout << "decrypted_plaintext.txt was not found or has open handles\n\n";
    }

    int option;
    bool running = true;
    string fileName;
    string keyFileName;
    vector<char> plainTextBuffer;
    vector<uint64_t> cipherTextBuffer;
    uint64_t key;

    while (running != false) {
        cout << "*******************************\n";
        cout << " 1 - Encrypt a file.\n";
        cout << " 2 - Decrypt a file.\n";
        cout << " 3 - Print file contents.\n";
        cout << " 4 - Turn off Verbose Logging.\n";
        cout << " 5 - Exit.\n";
        cout << " Enter your choice and press return: ";

        cin >> option;

        cin.ignore();

        cout << endl;

        switch (option)
        {
        case 1:
            cout << "Encrypt a file.\n\n";
            // rest of code here
            cout << " Enter the file name you would like to encrypt [default plaintext.txt] : ";
            getline(cin, fileName);
            cout << endl;
            cout << " Enter the file name that contains the hex formatted key [default key.txt] : ";
            getline(cin, keyFileName);

            //Read the files in this case the secret key is ab$ra85T

            if (!fileName.length() > 0)
                fileName = "plaintext.txt";
            if (!keyFileName.length() > 0)
                keyFileName = "key.txt";

            GetPlainTextFromFile(&plainTextBuffer, fileName);
            key = GetKeyFromFile(keyFileName);

            while (plainTextBuffer.size() > 0) {
                uint64_t plaintextBlock = GetBlockOfPlainText(&plainTextBuffer);
                plainTextBuffer.erase(plainTextBuffer.begin(), plainTextBuffer.begin() + 8);

                uint64_t block = encrypt_decrypt(&key, &plaintextBlock, true);

                WriteToFile(block, "ciphertext.txt");

            }

            break;
        case 2:
            cout << "Decrypt a file.\n";
            // rest of code here
            cout << " Enter the file name you would like to decrypt [default ciphertext.txt] : ";
            getline(cin, fileName);
            cout << endl;
            cout << " Enter the file name that contains the hex formatted key [default key.txt] : ";
            getline(cin, keyFileName);

            if (!fileName.length() > 0)
                fileName = "ciphertext.txt";
            if (!keyFileName.length() > 0)
                keyFileName = "key.txt";

            //Read the files in this case the secret key is ab$ra85T
            GetCipherTextFromFile(&cipherTextBuffer, fileName);
            key = GetKeyFromFile(keyFileName);

            while (cipherTextBuffer.size() > 0) {
                uint64_t ciphertextBlock = GetBlockOfCipherText(&cipherTextBuffer);
                cipherTextBuffer.erase(cipherTextBuffer.begin(), cipherTextBuffer.begin() + 1);

                uint64_t block = encrypt_decrypt(&key, &ciphertextBlock, false);

                

                WriteToFile(block, "decrypted_plaintext.txt");

            }
            break;
        case 3:
            cout << "Print file contents.\n";
            // rest of code here
            break;
        case 5:
            cout << "End of Program.\n";
            running = false;
            break;
        default:
            cout << "Not a Valid Choice. \n";
            cout << "Choose again.\n";
            cin.clear();
            cin >> option;
            break;
        }
    }
        
    //Read the files in this case the secret key is ab$ra85T
   // vector<char> plaintextBuffer = GetPlainTextFromFile();



    //uint64_t key = GetKeyFromFile();

    //Get the first 64 bit block of plaintext to encrypt/decrypt
    //Then remove those bits from the buffer so we dont read them again

    //while (plainTextBuffer.size() > 0) {
    //    uint64_t plaintextBlock = GetBlockOfPlainText(&plaintextBuffer);
    //    plaintextBuffer.erase(plaintextBuffer.begin(), plaintextBuffer.begin() + 8);

    //    uint64_t block = encrypt_decrypt(&key, &plaintextBlock, true);

    //    WriteToFile(block, "ciphertext.txt");

    //}


   // uint64_t plainText = encrypt_decrypt(&key, &cipherText, false);

    


    cout << "Good Bye from PSU_CRYPT!" << endl;
    return 0;
}




