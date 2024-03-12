#include <iostream>
#include "genlib.h" 
#include "simpio.h" 

int changeLettersToNumbers(string &surname) {
	
	// for each char in surname
	for (int index = 1; index < surname.length(); index++) {
		
		// creates a char to store the char of the current index
		char ch = surname[index];
	
		// turn the char into a '0'
		if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' || ch == 'h' || ch == 'w' || ch == 'y') {
			
			surname[index] = '0';
		}
		
		// turn the char into a '1'
		else if (ch == 'b' || ch == 'f' || ch == 'p' || ch == 'v') {
			
			surname[index] = '1';
		}
		
		// turn the char into a '2'
		else if (ch == 'c' || ch == 'g' || ch == 'j' || ch == 'k' || ch == 'q' || ch == 's' || ch == 'x' || ch == 'z') {
			
			surname[index] = '2';
		}
		
		// turn the char into a '3'
		else if (ch == 'd' || ch == 't') {
			
			surname[index] = '3';
		}
		
		// turn the char into a '4'
		else if (ch == 'm' || ch == 'n') {
			
			surname[index] = '4';
		}
		
		// turn the char into a '5'
		else if (ch == 'l') {
			
			surname[index] = '5';
		}
		
		// turn the char into a '6'
		else if (ch == 'r') {
			
			surname[index] = '6';
		}
	}
	return 0;
}

int truncateDuplicateNumbers(string &surname) {
	
	// for each char in surname
	for (int index = 0; index < surname.length() - 1; ) {
        
		// create a char to store the current char
		char currentChar = surname[index];
        
		// create a char to store the next char
		char nextChar = surname[index + 1];
		
		// if the current char is equal to the next char and the string lenght is > 4
        if (currentChar == nextChar && surname.length() > 4) {
            
			// remove the consecutive char
			surname.erase(index + 1, 1);
       
		} else {
            
			index++;
        }
    }
	return 0;
}

int removeZerosIfNecessary(string &surname) {

	// if surname string lenght is > 4
	if (surname.length() > 4) {
		
		// for each char in surname
		for (int index = 1; index < surname.length(); index++) {
			
			// create a char to store the current char
			char ch = surname[index];
			
			// if surname string lenght is > 4
			if (surname.length() > 4) {
				
				// if the char is equal to '0'
				if (ch == '0') {
					
					// remove '0' from the string
					surname.erase(index, 1);
				}
			}
		}
	}
	return 0;
}

int soundex(string &surname) {
	
	// Turn the first char of surname into a upper case
	surname[0] = toupper(surname[0]);
	
	// convert all letters to numbers except the first
	changeLettersToNumbers(surname);
	
	// Remove any consecutive duplicate
	truncateDuplicateNumbers(surname);
	
	// Remove zeros to make soundex surname length equals 4
	removeZerosIfNecessary(surname);

	return 0;
}
	
int main() {
	
	cout << "Enter a surname: ";

    // get the surname from the user
	string surname = GetLine();

    // create a string to store the original surname
	string surnameBackup = surname;

    // apply the soundex algorithm in the surname
	soundex(surname);

    // print the soundexcode for the surname entered
	cout << "Soundex code for " << surnameBackup << " is " << surname;

	return 0;
}