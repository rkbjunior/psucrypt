# psucrypt
PSU_CRYPT is a combination of twofish and skip jack for encrypting and decrypting plain text using a 64 bit key.

When the application is started, a simple menu is displayed where you can choose to 
1. Encrypt a file
2. Decrypt a file
3. Change logging options
4. Exit the program

A default `key.txt` and `plaintext.txt` have been provided. If you select to encrypt/decrypt a file, default values
can be used simply by pressing enter when asked to input the file names. After encryption an intermediae file is
created called `ciphertext.txt` and after decryption a filed called `decryption_plaintext.txt` is created with the
unencrypted original plain text.

The key.txt file contains a 64 bit key in base 16 (hex) format without a 0x prefix that is sometimes used when
representing hex values. If you choose to supply your own key, the key must be 64 bits in length and in base 16 (hex)
format.

The plain text is read in entirely and then encrypted/decrypted in 64 bit blocks at a time. The plaintext should be
standard ASCII only, no special values. The outputed ciphertext will space delimined hex values.

