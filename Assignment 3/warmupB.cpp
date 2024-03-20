#include <iostream>
#include "genlib.h" 
#include "simpio.h" 
#include <fstream>
#include "vector.h"


bool RecSum(Vector<int> & nums, int index, int sumSoFar, int target) {
    
	// success base case
	if (sumSoFar == target) return true; 	
	
	// failure base case
    if (index == nums.size()) return false;	

	// recursive case, try next element both in and out of the sum
    return RecSum(nums, index+1, sumSoFar, target) ||  
           RecSum(nums, index+1, sumSoFar+nums[index], target);
}

bool CanMakeSum(Vector<int> & nums, int targetSum){
    
	return RecSum(nums, 0, 0, targetSum);
}

int main() {
	
	while (true) {	

		Vector <int> nums;

		nums.add(3);

		nums.add(7);

		nums.add(1);

		nums.add(8);

		nums.add(-3);

		cout << "Enter a number to check if is possible: ";

		int targetSum = GetInteger();

		bool canMakeSum = CanMakeSum(nums, targetSum);

			if (canMakeSum == true) {
				
				cout << "the sum is possible" << endl;
			}

			else {

				cout << "the sum isn't possible" << endl;
			}
	}
}