#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "fileio.h"
#include "psu_crypt.h"
#include "globals.h"

using namespace std;

int main()
{
    //If the ciphertext.txt exists, delete it.
    if (LOGGING > 0)
        cout << "Cleaning up some stuff...\n";

    if (remove("ciphertext.txt") == 0) {
        if (LOGGING > 0)
            cout << "Deleted ciphertext.txt\n" ;
    }
    else {
        if (LOGGING > 0)
            cout << "ciphertext.txt was not found or has open handles\n";
    }

    //If the decrypted_plaintext.txt exists, delete it.
    if (remove("decrypted_plaintext.txt") == 0) {
        if (LOGGING > 0)
            cout << "Deleted decrypted_plaintext.txt\n\n";
    }
    else {
        if (LOGGING > 0)
            cout << "decrypted_plaintext.txt was not found or has open handles\n\n";
    }

    int option;
    bool running = true;
    string fileName;
    string keyFileName;
    vector<char> ptextBuffer;
    vector<uint64_t> cipherTextBuffer;
    uint64_t key;

    fileio file;
    psu_crypt pcrypt;

    while (running != false) {
        cout << "*******************************\n";
        cout << " 1 - Encrypt a file.\n";
        cout << " 2 - Decrypt a file.\n";
        cout << " 3 - Turn off Verbose Logging.\n";
        cout << " 4 - Exit.\n";
        cout << " Enter your choice and press return: ";

        cin >> option;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        

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

            
            file.GetPlainTextFromFile(&ptextBuffer, fileName);
            key = file.GetKeyFromFile(keyFileName);

            while (ptextBuffer.size() > 0) {
                uint64_t plaintextBlock = pcrypt.GetBlockOfPlainText(&ptextBuffer);
                ptextBuffer.erase(ptextBuffer.begin(), ptextBuffer.begin() + 8);

                uint64_t block = pcrypt.encrypt_decrypt(&key, &plaintextBlock, true);

                file.WriteToFile(block, "ciphertext.txt");

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
            file.GetCipherTextFromFile(&cipherTextBuffer, fileName);
            key = file.GetKeyFromFile(keyFileName);

            while (cipherTextBuffer.size() > 0) {
                uint64_t ciphertextBlock = cipherTextBuffer.at(0);
                cipherTextBuffer.erase(cipherTextBuffer.begin(), cipherTextBuffer.begin() + 1);

                uint64_t block = pcrypt.encrypt_decrypt(&key, &ciphertextBlock, false);

                file.WriteToFile(block, "decrypted_plaintext.txt");

            }
            break;
        case 3:
            cout << "Change Log Options [0: NONE, 1: INFO, 2: VERBOSE]\n";
            cin >> LOGGING;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            cout << endl;
            break;
        case 4:
            cout << "End of Program.\n";
            running = false;
            break;
        default:
            cout << "Not a Valid Choice. \n";
            cout << "Choose again.\n\n";
            break;
        }
    }
        
    cout << "Good Bye from PSU_CRYPT!" << endl;
    return 0;
}




