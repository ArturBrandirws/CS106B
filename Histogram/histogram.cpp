#include <iostream>
#include "genlib.h" 
#include "simpio.h" 
#include <fstream>
#include "vector.h"

int scoreVector(ifstream &in, Vector <int> &listOfScores) {
	
	while (true) {
		
		string line;

		getline(in, line);
		
		if (in.fail()) break;

		listOfScores.add(StringToInteger(line));
	}
	return 0;
}

int initializeAppearanceRateVector(Vector <int> &appearanceRate) {
	
	cout << "initializing appearance rate vector" << endl;

	for (int index = 0; index < appearanceRate.size(); index++) {
		
		appearanceRate[index] = 0;
	}

	return 0;
}

int coutHistogram(Vector <int> &listOfScores, Vector <int> &appearanceRate) {
	
	int startingNum = 10;

	int endingNum = 19;

	cout << "starting to cout" << endl;

	for (int index = 0; index < 9; index++) {
		
		cout << startingNum << "-" << endingNum << ": " << appearanceRate[index] << endl;

		startingNum += 10;

		endingNum += 10;
	}

	return 0;
}

int getScores(ifstream &in) {
	
	Vector <int> listOfScores(11);

	scoreVector(in, listOfScores);

	Vector <int> appearanceRate(8);
	
	initializeAppearanceRateVector(appearanceRate);

	coutHistogram(listOfScores, appearanceRate);

	return 0;
}
	
int main() {
	
	ifstream in;

	ofstream out;
	
	while (true) {

		cout << "Enter the name of the file: ";

		string filename = GetLine();

		in.open(filename.c_str());
		
		if (in.fail()) {

			cout << "file didn't open" << endl;
		}

		else {
			
			cout << "opening " << filename << endl;
			break;
		}
	}

	getScores(in);

	return 0;
}