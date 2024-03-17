/* 
 * File: markovModel.cpp
 * --------------
 */

#include <iostream>
#include "genlib.h" 
#include "simpio.h" 
#include <fstream>
#include "vector.h"
#include "extgraph.h"
#include "map.h"

Map <int> getCharSequence(Vector <string> &charsInSeed, int order) {
	
	// create the map to store char frequency
	Map<int> charFrequency;
	
	// if the order choosed by the user is 1
	if (order == 1) {
		
		// for each char in the source file
		for (int index = 0; index < charsInSeed.size(); index++)
			
			// if the char is already in the map
			if (charFrequency.containsKey(charsInSeed[index])) {
	            
				// define the count of the char
				int count = charFrequency[charsInSeed[index]];
				
				// increase the count of the char
				charFrequency[charsInSeed[index]] = count + 1;
			}

			// if the char isn't in the map
			else {
	            
				// add the char to the map
				charFrequency[charsInSeed[index]] = 1;
			}
	}

	// if the order choosed by the user is 2 or bigger
	else if (order >= 2) {
		
		// for each char in the source file
		for (int index = 0; index < charsInSeed.size() - order + 1; index++) {
			
			// create a empty string
			string sequence = "";
			
			// repeat based on the order choosed
			for (int i = 0; i < order; i++) {
				
				sequence += charsInSeed[index + i];
			}
			if (charFrequency.containsKey(sequence)) {
				
				int count = charFrequency.getValue(sequence);
				
				charFrequency.add(sequence, count + 1);
			}
			else {

				charFrequency.add(sequence, 1);
			}
		}
	}
	return charFrequency;
}
Map <int> readFile(ifstream &in, int order) {
	
	// create a vector to store all ordered chars from the source
	Vector <string> charsInSeed;
	
	// declare a map to store frequency of each char
	Map <int> charFrequency;
	
	// declare a char ch
	char ch;
	
	while (true) {
	    
		// get the char from the source
		in.get(ch);
	     
		// if the source doesnt have new chars
		if (in.fail()) break;
	       
		// Convert char to string
		string chStr(1, ch);
		
		// add the char/string to the vector
		charsInSeed.add(chStr);
	}
	
	// create the char frequency map
	charFrequency = getCharSequence(charsInSeed, order);
	
	return charFrequency;
}

int getFileSize(ifstream &in) {
	
	char ch;

	int count = 0;

	while (true) {

		in.get(ch);
		
		if (in.fail()) break;

		count++;
	}

	return count;
}

Map <double> getCharsFrequency(ifstream &in, int numOfChars, int order) {

	Map <int> charFrequencyAtfile = readFile(in, order);

	Map <int>::Iterator itr = charFrequencyAtfile.iterator();

	Map <double> charProbability;
		
	while (itr.hasNext()) {
			
		string key = itr.next();

		double count = charFrequencyAtfile.getValue(key);

		double probability = count / numOfChars;

		charProbability.add(key, probability);

	}
	
	return charProbability;
}

char generateRandomChar(Map <double> &charProbability) {
    
	// Generate a random probability between 0 and 1
    double randomProbability = (double)rand() / RAND_MAX;

    // Accumulate probabilities until we find the character
    double cumulativeProbability = 0;
	
	// create an iterator to go through the map
    Map <double>::Iterator itr = charProbability.iterator();
	
	// while map has more keys
    while (itr.hasNext()) {

		// create a string with the key
        string key = itr.next();
		
		// add the probability of the char to the cumulative Probability
        cumulativeProbability += charProbability[key];
        
		// if the random number generated <= than the accumulated probability
        if (randomProbability <= cumulativeProbability) {
			
			// Return the character
            return key[0];
        }
    }

    // In case of unexpected failure
    return 0;
}

string generateRandomText(Map <double> &charProbability, int outputTextSize, int order) {
    string randomText;

    if (order == 1) {
        for (int i = 0; i < outputTextSize; i++) {
            char randomChar = generateRandomChar(charProbability);
            randomText += randomChar;
        }
    } else if (order >= 2) {
        string key = "";
        while (randomText.length() < outputTextSize) {
            char randomChar = generateRandomChar(charProbability);
            key += randomChar;
            if (key.length() > order) {
                key = key.substr(1); // Remove the first character if the key length exceeds order
            }
            if (charProbability.containsKey(key)) {
                double randomProbability = (double)rand() / RAND_MAX;
                if (randomProbability <= charProbability[key]) {
                    randomText += key[key.length() - 1]; // Add the last character of the key to random text
                }
            }
        }
    }
    return randomText;
}


int analizeOrder(ifstream &in, int numOfChars) {
	
	// determine the size of the random output
	int outputTextSize = 20;
	
	cout << "What order of analysis? (a number from 1 to 10): ";
	
	// get an int from the user to determine which analise will be used
	int order = GetInteger();
	
	// create a map to save the probability of each sequence or char based on the frequency
	Map <double> charProbability = getCharsFrequency(in, numOfChars, order);
	
	// create a random string using the probability of each char
	string randomText = generateRandomText(charProbability, outputTextSize, order);
	
	// print the random text
	cout << "Random text: " << randomText;

	return 0;
}

int main (){
	
	// create a variable to access files
	ifstream in;
	
	// while the user enter the name of the file
	while (true) {
	
		cout << "Enter the name of the file: ";
		
		// get a string from the user to access the file by name
		string filename = GetLine();
		
		// open the file with the name typed
		in.open(filename.c_str());
		
		// if the file isn't located
		if (in.fail()) {
			
			// clean the file varible
			in.clear();
			
			cout << "file didn't open. Please type an valid name" << endl;
		}
		
		// if the file opens
		else {
			
			cout << "opening " << filename << endl;
			break;
		}
	}
	
	// create a int to store the amount of chars in the source file
	int numOfChars = getFileSize(in);
		
	// clean the file varible
	in.clear();
	
	// return the file reader to the first char
	in.seekg(0);
	
	// analizes the seed with the desired type of model
	analizeOrder(in, numOfChars);
	
	return 0;
}	
