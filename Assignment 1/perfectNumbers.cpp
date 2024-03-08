#include "genlib.h"
#include "simpio.h"
#include <iostream>

// Function to check if a number is perfect
bool isPerfect(int num) {
	
	// start the sum with 1
    int sum = 1; 

	bool perfect = false;

    // Check for divisors up to the square root of the number
    for (int divisor = 2; divisor * divisor <= num; divisor++) {
        
		// if the number divided by the divisor result an integer
		if (num % divisor == 0) {
            
			// add the divisor to the sum
			sum += divisor;
            
			// used to avoid counting the number
			if (divisor != num / divisor) {
               
				sum += num / divisor;
            }
			
			// if the sum is equal to the number
			if (sum == num) {
				
				perfect = true;

				return perfect;
			}
        }
    }

    // Check if the sum of divisors is equal to the original number
    return perfect;
}

int main() {
    
    // print the obejctive of the code
	cout << "Perfect numbers before 10000 are:" << endl;

    // for every number between 2 and 10000
    for (int number = 2; number <= 10000; ++number) {
        
		// check if the number is perfect
		if (isPerfect(number)) {
            
			// print the perfect number
			cout << number << " " << endl;
        }
    }
    return 0;
}