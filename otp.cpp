/*
===================================================================================
| Program to implement the One-Time Pad Cryptosystem. This program reads a file   |
| containing only lowercase or uppercase letters and spaces (assumed newline at   |
| end), encrypts the file using random numbers from rand() seeded with time(NULL) |
| and saves the encrypted text and the random keys generated to separate files.   |
| All file names are provided by the user running the program from the command    |
| line.                                                                           |
|                                                                                 |
| Usage                                                                           |
| otp -<option> <read_from> <write_to> <keys>                                     |
|                                                                                 |
| <option>                                                                        |
| e					: encrypt                                                             |
| d					: decrypt                                                             |
| <read_from>       : file to read from (original text or encrypted text          |
| <write_to>        : file to write data to (encrypted text or decrypted text     |
| <keys>            : secret random keys to be generated or read                  |
===================================================================================
|                                Public Functions                                 |
| // Generates two files: one containing the encrypted message and the other      |
| // containing the secret randomly generated keys                                |
| void Encrypt(char* unencrypted_file, char* output_file, char* secret_file );    |
|                                                                                 |
| // Generates one file containing the decrypted message                          |
| void Decrypt(char* encrypted_file, char* output_file, char* secret_file );      |
|                                                                                 |
| // Auxiliary function, reads a file to a string                                 |
| string Read(char* file_name);                                                   |
|                                                                                 |
| // Auxiliary function, writes a string to a file (overwrites the file)          |
| void Write(char* file_name, string message);                                    |
===================================================================================
*/

/* including libraries needed for program */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
/* number of possible characters */
#define MAP_SIZE 53                   

using namespace std;
/* table with the mapping for the characters. the first of them is at index 1
and the last one is at index MAX_SIZE */
static const char mapping[] = { '\0', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm',
'n', 'o', 'p', 'q', 'r', 's', 't',
'u', 'v', 'w', 'x', 'y', 'z', 'A',
'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O',
'P', 'Q', 'R', 'S', 'T', 'U', 'V',
'W', 'X', 'Y', 'Z', ' ' };

/* generates two files: one containing the encrypted message and the other
containing the secret randomly generated keys */
void Encrypt(char* unencrypted_file, char* output_file, char* secret_file);

/* generates one file containing the decrypted message */
void Decrypt(char* encrypted_file, char* output_file, char* secret_file);

/* auxiliary function, reads a file to a string */
string Read(char* file_name);

/* auxiliary function, writes a string to a file (overwrites the file) */
void Write(char* file_name, string message);

/*
================================================================
| Interprets the user command given from the command line.     |
|                                                              |
| @param    argc     number of arguments (given by the system) |
| @param    argv     value of arguments (given by the system)  |
| @return   0        if ends without errors                    |
================================================================
*/

int main(int argc, char* argv[])
{
	/* check correct number of arguments */
	if (argc == 5){
		/* check for encryption */
		if ((argv[1][0] == '-') && (argv[1][1] == 'e'))
		{
			Encrypt(argv[2], argv[3], argv[4]);
		}
		/* check for decryption */
		else if ((argv[1][0] == '-') && (argv[1][1] == 'd'))
		{
			Decrypt(argv[2], argv[3], argv[4]);
		}
		else{
			std::cout << "Invalid operation" << endl;
		}
	}
	else{
		/* show usage and help if wrong syntax or invalid amount of arguments supplied */
		std::cout << "Invalid number of arguments supplied." << endl;
		std::cout << "Usage:" << endl;
		std::cout << "otp - <option> <read_from> <write_to> <keys>" << endl;
		std::cout << "" << endl;
		std::cout << "<option>" << endl;
		std::cout << "e            : encrypt" << endl;
		std::cout << "d            : decrypt" << endl;
		std::cout << "<read_from>  : file to read from original text or encrypted text" << endl;
		std::cout << "<write_to>   : file to write data to encrypted text or decrypted text" << endl;
		std::cout << "<keys>       : secret random keys to be generated or read" << endl;
	}
	return 0;
}

/*
===================================================================================
| Encrypts the text in a file. Generates two files containing the encrypted       |
| file and the randomly generated keys to decrypt the file. the file is           |
| assumed to contain only letters and spaces (newline at end).                    |
|                                                                                 |
| @param    unencrypted_file   name of the file to encrypt                        |
| @param    output_file        name of the file to overwrite the encrypted text   |
| @param    secret_file        name of the file to overwrite the secret keys      |
===================================================================================
*/

void Encrypt(char* unencrypted_file, char* output_file, char* secret_file){

	/* read the text from the file */
	string unencrypted = Read(unencrypted_file);
	string encrypted;
	/* array to contain the keys */
	int* keys = new int[unencrypted.length()];
	/* string representation of the keys (permits the use of standard Write()) */
	string secret_keys;

	/* check for empty file */
	if (!unencrypted.empty()){
		/* seed random number generator */
		srand(time(NULL));
		/* traverse the text to encrypt every character */
		for (unsigned int i = 0; i < unencrypted.length(); ++i){
			/* compare with mapping */
			for (unsigned int j = 1; j <= MAP_SIZE; ++j){
				/* match found */
				if (unencrypted[i] == mapping[j]){
					/* generate random key */
					keys[i] = (int)(1 + (rand() % 53));
					/* append key to string representation of keys */
					ostringstream temp;
					temp << keys[i];
					secret_keys.append(temp.str());
					if (i < unencrypted.length()) secret_keys.append(" ");
					/* check wrap condition, append encrypted character */
					if (j + keys[i] > 53){
						encrypted += mapping[j + keys[i] - 53];
					}
					else{
						encrypted += mapping[j + keys[i]];
					}
					break;
				}
			}
		}
		/* save files */
		Write(output_file, encrypted);
		Write(secret_file, secret_keys);
	}
	else{
		/* signal for empty file (nothing to encrypt) */
		std::cout << "ERROR: Empty file" << endl;
	}
}

/*
===================================================================================
| Decrypts the text in a file. Generates one file containing the decrypted text.  |
| The file is assumed to contain only letters and spaces (newline at end).        |
|                                                                                 |
| @param    encrypted_file     name of the file to decrypt                        |
| @param    output_file        name of the file to overwrite the decrypted text   |
| @param    secret_file        name of the file to read the secret keys           |
===================================================================================
*/

void Decrypt(char* encrypted_file, char* output_file, char* secret_file){

	/* read encrypted text */
	string encrypted = Read(encrypted_file);
	string unencrypted;
	/* read keys */
	string secret_keys = Read(secret_file);
	/* convert keys to integers */
	vector <int> keys;
	stringstream s(secret_keys);
	string temp;
	while (s >> temp){
		int number;
		istringstream convert(temp);
		convert >> number;
		keys.push_back(number);
	}

	/* check for empty file */
	if (!encrypted.empty()){
		/* traverse the text to decrypt every character */
		for (unsigned int i = 0; i < encrypted.length(); ++i){
			/* compare with mapping */
			for (int j = 1; j <= MAP_SIZE; ++j){
				/* match found */
				if (encrypted[i] == mapping[j]){
					/* check wrap condition, append decrypted character */
					if (j - keys[i] < 1){
						unencrypted += mapping[j - keys[i] + 53];
					}
					else{
						unencrypted += mapping[j - keys[i]];
					}
					break;
				}
			}
		}
		/* save file */
		Write(output_file, unencrypted);
	}
	else{
		/* signal for empty file (nothing to decrypt) */
		std::cout << "ERROR: The file specified is empty." << endl;
	}
}

/*
===========================================================
| Reads an entire file to a string.                       |
|                                                         |
| @param    file_name          name of the file to read   |
| @return   string             text read from file        |
===========================================================
*/

string Read(char* file_name){
	/* file object */
	std::ifstream file_stream(file_name, std::ifstream::in);
	std::string message;
	/* check for successful opening */
	if (file_stream.is_open())
	{
		std::cout << "Attempting to read from file \"" << file_name << "\"...";
		/* read file character by character */
		while (!file_stream.eof()) message.push_back(file_stream.get());
		/* signal success */
		std::cout << " Done!" << endl;
		/* close file */
		file_stream.close();
	}
	/* signal error opening the file */
	else cout << "ERROR: Unable to open file specified, please check the path." << endl;

	return message;
}

/*
================================================================
| Overwrites a string to a file.                               |
|                                                              |
| @param    file_name          name of the file to overwrite   |
| @param    message            string to be written            |
| @return   string             text read from file             |
================================================================
*/

void Write(char* file_name, string message){
	/* file object */
	ofstream file_stream;
	file_stream.open(file_name, fstream::out);
	/* Check for opened file */
	if (file_stream.is_open())
	{
		std::cout << "Attempting to save to file \"" << file_name << "\"...";
		/* write text/keys */
		file_stream << message << endl;
		/* close file */
		file_stream.close();
		/* signal success */
		std::cout << " Done!" << endl;
	}
	else
	{
		/* signal error in opening */
		cout << "Error: Could not save the file." << endl;
	}
}
