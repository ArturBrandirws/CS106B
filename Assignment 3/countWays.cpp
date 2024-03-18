#include <iostream>
#include "genlib.h" 
#include "simpio.h" 

int CountWays(int numStairs) {
	
	// Base cases
    if (numStairs <= 1)
        return 1;
    
    // Recursive cases
    // Taking a small stride
    int ways1 = CountWays(numStairs - 1);
    
	// Taking a large stride
    int ways2 = CountWays(numStairs - 2);
    
	// return the sum of ways
    return ways1 + ways2;
}

int main() {
	
	// loop to enter infinite different numbers of stairs
	while (true) {

		cout << endl;
		
		cout << "This program count the ways possible to climb a stair" << endl;
		
		cout << "Enter how many steps are in the staircase: ";
		
		// get the number of stairs from the user
		int numStairs = GetInteger();
		
		// calculate the ways
		int count = CountWays(numStairs);
		
		cout << endl;

		cout << "There is " << count << " ways to climb that stair";
	}
}