# psucrypt
PSU_CRYPT is a combination of twofish and skip jack for encrypting and decrypting plain text using a 64 bit key.

When the application is started, it looks in the local directory for a plaintext.txt and key.txt file and reads them into memory.

The key.txt file contains a 64 bit key in base 16 (hex) format. 

The plain text is read in entirely and then encrypted/decrypted in 64 bit blocks at a time. 

