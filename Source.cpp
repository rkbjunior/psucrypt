#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include "psu_crypt.h"
#include "helpers.h"
#include "fileio.h"

using namespace std;

int LOGGING = 1;

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
                uint64_t ciphertextBlock = cipherTextBuffer.at(0);
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




