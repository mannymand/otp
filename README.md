# One-Time-Pad
Program to implement the One-Time Pad Cryptosystem. This program reads a file containing only lowercase or uppercase letters and spaces (assumed newline at end), encrypts the file using random numbers from rand() seeded with time(NULL) and saves the encrypted text and the random keys generated to separate files. All file names are provided by the user running the program from the command line.              

Usage                                                                           
otp -`<option>` `<read_from>` `<write_to>` `<keys>`                                  

`<option>`                                                                       
-e					  : encrypt                                                     
-d					  : decrypt                                                     
`<read_from>` : file to read from (original text or encrypted text)       
`<write_to>`  : file to write data to (encrypted text or decrypted text)     
`<keys>`      : secret random keys to be generated or read)
