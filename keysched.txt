The Key Schedule K(x)  
Encryption: 
Input: A number x and it is assumed that K() has access to the current, stored, 64 bit key K. 
The key is 64 bits and so 8 bytes long. 
Label the bytes 0 through 7 so K = k7, k6, k5, k4, k3, k2, k1, k0. 
Left circular rotate K by 1 bit and store this rotated value as the new key K’.   
Output: 
The x mod 8 byte of the key K’. 
So if x = 18 then since 18 mod 8 = 2 output the third byte (k2) of K. 
This allows us to output the first byte when, for example x = 16 and 16 mod 8 = 0, so the output byte is k0.




Decryption: 
Input: 
A number x and it is assumed that K() has access to the current, stored, 64 bit key K. 
The key is 64 bits and so 8 bytes long. Label the bytes 0 through 7 as for encryption.  
Now z is the x mod 8 byte of the key K. 
Right rotate K by 1 bit and store this value as the new key K’. 
Unlike encryption, the subkey is gotten before the rotation of the key.
Output: z.  

Instead you can generate ALL subkeys when the program starts, in this case decryption keys are just the 
keys used in reverse order.
