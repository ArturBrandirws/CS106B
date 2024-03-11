#include <iostream>
#include "random.h"
#include "genlib.h" 
#include "simpio.h" 

double calculateResults(int numberOfVotes, double spreadVotes, double errorPercentage) {
	
	// create a variable to store votes for A
	double candidateA = 0;
	
	// create a variable to store votes for B
    double candidateB = 0;
	
	// for each vote
    for (int vote = 1; vote <= numberOfVotes; vote++) {
	
		// create a random number between 0 and 1
        double randomNum = RandomReal(0.0, 1.0);
		
		// create a variable with the basic chance of a candidate get vote
		double basicChance = 0.5;
		
		// returns true with the same odd that the user typed in error
		double error = RandomChance(errorPercentage);
		
		// if the error is false
		if (error = false) {

			// if the random number is lower than the chance of A get a vote
			if (randomNum < (basicChance + spreadVotes)){
				
				candidateA = candidateA + 1;
			}
			
			// if the random number is bigger than the chance of B get a vote
			else if (randomNum > (basicChance - spreadVotes)) {
				
				candidateB = candidateB + 1;
			}	
		}
		
		// if the error is true
		else {
			
			// if the random number is lower than the chance of B get a vote
			if (randomNum < (basicChance + spreadVotes)){
				
				candidateB = candidateB + 1;
			}
			
			// if the random number is bigger than the chance of A get a vote
			else if (randomNum > (basicChance - spreadVotes)) {
				
				candidateA = candidateA + 1;
			}	
		}
    }
	
	// calculate the percentage of votes in A
	double percentageA = (candidateA / numberOfVotes) * 100;
	
	// print the percentage of votes in A
	cout << "percentage of A: " << percentageA << "%" << endl;
	
	// calculate the percentage of votes in B
	double percentageB = (candidateB / numberOfVotes) * 100;

	// print the percentage of votes in A
	cout << "percentage of B: " << percentageB << "%" << endl;

	return 0.0;
}
int main() {
	
	cout << "Insert the number of votes: ";
	
	// get the number of votes
	int numberOfVotes = GetInteger();

	cout << "Insert the spread of votes: ";

	double spreadVotes;
	
	// get the votation spread
	std::cin >> spreadVotes;

	cout << "Insert the error percentage of votes: ";

	double errorPercentage;
	
	// get the error percentage
	std::cin >> errorPercentage;
	
	// calculate Results
	calculateResults(numberOfVotes, spreadVotes, errorPercentage);

	return 0;
}
