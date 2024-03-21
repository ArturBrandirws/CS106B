#include <iostream>
#include "simpio.h"
#include "lexicon.h"

// print all possible words
void printWords(string wordPrefix, Lexicon &lex){
    
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	
	// if the prefix is already a complete word
    if(lex.containsWord(wordPrefix)) cout << wordPrefix << " ";
	
	// if the prefix is a valid prefix for any word
    if(lex.containsPrefix(wordPrefix)){
		
		// for every letter in the alphabet
		for(int i = 0; i < alphabet.length(); i++){
			
			// checks if the word prefix fits with the letters
			printWords(wordPrefix + alphabet.at(i), lex);
        } 
    }
}

// return the letters equivalent to the number
string digitToLetters(int digit){
    
	if (digit == 2) {
        
		return "abc";
	}

	else if (digit == 3) {
        
		return "def";
	}

	else if (digit == 4) {
        
		return "ghi";
	}

	else if (digit == 5) {
        
		return "jkl";
	}

	else if (digit == 6) {
        
		return "mno";
	}

	else if (digit == 7) {
        
		return "pqrs";
	}

	else if (digit == 8) {
        
		return "tuv";
	}

	else if (digit == 9) {
        
		return "wxyz";
	}

	else {
		
		return "";
	}
}

// recursive function
void analizeSequence(string digits, Lexicon & lex, string wordPrefix){
    
	// if the word prefix is finished
    if(digits.length() == 0){
       
		// check if the word is valid then print it
		printWords(wordPrefix, lex);
	}

	// recursive function that changes the digits for letters
	else {
		
		// create a string with the possible letters typed
        string possibleLetters = digitToLetters(int(digits.at(0)));
        
		// loop for each letter in the string
		for (int j = 0; j < possibleLetters.length(); j++){
            
			// use the recursion to form the possible words
			analizeSequence(digits.substr(1), lex, wordPrefix + possibleLetters.at(j));
        };
    }
}


void ListWords(string digits, Lexicon & lex) {

	// analizes the sequence and create the possible words with the sequence
    analizeSequence(digits, lex, "");
}

int main ()
{
	// use the lexicon.cpp to store all data from "lexicon.dat"
    Lexicon lex("lexicon.dat");

	cout << "This program shows the possible word formed by a sequence of numbers" << endl;
	
	cout << "Enter a sequence of numbers: ";

	// get the sequence of numbers from the user
    string sequence = GetLine();
	
	cout << endl;
	cout << "Looking for valid words for the sequence: " << sequence << endl;
	
	// check the input and print possible words
    ListWords(sequence,lex);

    return 0;
}