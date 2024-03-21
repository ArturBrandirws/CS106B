#include "vector.h"
#include "genlib.h"
#include "simpio.h"
#include <iostream>

int CutStock(Vector<int> & pipeLengths, int startRequest, int totalLength) {
	
	// if the recursion finished the sum
	if (startRequest >= pipeLengths.size()) {
		
		return totalLength;
	}
	
	// sets the request pipe length
	int requestLength = pipeLengths[startRequest];
	
	// add to the total length
	totalLength += requestLength;
	
	// recall the function with the new params
	CutStock(pipeLengths, startRequest + 1, totalLength);
}

int main() {
	
	// create a vector to store pipes lengths
	Vector <int> pipeLengths;
	
	// add the pipe lenghts to the vector
	pipeLengths.add(4);
	pipeLengths.add(3);
	pipeLengths.add(4);
	pipeLengths.add(1);
	pipeLengths.add(7);
	pipeLengths.add(8);
	
	// declare the size of the pipe
	int stockPipeLength = 10;
	
	// get the total length of pipes from a recursive function
	int total = CutStock(pipeLengths, 0, 0);
	
	// used in the while loop
	int count = 0;
	
	// sum the length of the pipe until it exceds the total length
	while (count < total) {
		
		// add the length to the count
		count += stockPipeLength;
	}
	
	// create a variable with the amout of pipes needed
	int amountOfPipes = count / stockPipeLength;

	cout << "The amount of pipes needed is: " << amountOfPipes << endl;
}