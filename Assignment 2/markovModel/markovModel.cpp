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
				
				// add in the sequence the next chars depending on what order was selected
				sequence += charsInSeed[index + i];
			}
			// if the sequence already exist
			if (charFrequency.containsKey(sequence)) {
				
				// create a count for the sequence
				int count = charFrequency.getValue(sequence);
				
				// add 1 in the count
				charFrequency.add(sequence, count + 1);
			}
			
			// if is the first time of the sequence
			else {
				
				// add the pair of key and value for the sequence
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
	
	// initialize a char 
	char ch;
	
	// create a count
	int count = 0;
	
	while (true) {
		
		// get the next char in the file
		in.get(ch);
		
		// if the file has ended, break
		if (in.fail()) break;
		
		// add 1 in count
		count++;
	}

	return count;
}

Map <double> getCharsFrequency(ifstream &in, int numOfChars, int order) {
	
	// Get character frequency map from file
    Map<int> charFrequencyAtfile = readFile(in, order);
    
	// Create an iterator to go through the frequency
    Map<int>::Iterator itr = charFrequencyAtfile.iterator();
    
	// Create a map to store characters probability
    Map<double> charProbability;

    // Calculate probability for each character
    while (itr.hasNext()) {
        
		// create a string with the char or string
		string key = itr.next();

		// get the value with the key(char or string)
        double count = charFrequencyAtfile.getValue(key);

		// calculate the probability
        double probability = count / numOfChars;

		// add to the probability map
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
    
	// initializes a string to be the random text
	string randomText;
	
	// if the order selected was 1
    if (order == 1) {

		// for each char in the output text
        for (int i = 0; i < outputTextSize; i++) {
            
			// get a random char
			char randomChar = generateRandomChar(charProbability);

			// add the random char to the string
            randomText += randomChar;
        }
	}
    
   // If the order selected was 2 or more
   else if (order >= 2) {
    
	// Create an empty string to store character sequences
    string key = "";
    
	// Generate random text based on chosen order
    while (randomText.length() < outputTextSize) {
        
		// get a random sequence
		char randomChar = generateRandomChar(charProbability);
        
		// add the sequence to the key
		key += randomChar;
        
		// If the length of the character sequence exceeds the order
        if (key.length() > order) {
            
			// Remove the first character to maintain the sequence length
            key = key.substr(1); // Remove the first character if the key length exceeds order
        }
        
		// Check if the character sequence is in the probability map
        if (charProbability.containsKey(key)) {
            
			// Generate a random probability
            double randomProbability = (double)rand() / RAND_MAX;
            
			// If the random probability is less than or equal to the probability of the sequence
            if (randomProbability <= charProbability[key]) {
                
				// Add the last character of the sequence to the random text
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