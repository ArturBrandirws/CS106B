#include <iostream>
#include <vector.h>

int part(Vector <int> &list, int first, int last)
{
  // takes the last index as the pivot
  int pivot = list[last];
  
  int lastIndex = first - 1;
  
  for (int index = first; index <= last; index++)
  {
    //If current element is smaller than the pivot
    if (list[index] < pivot)
    {
      //Increment index of smaller element
      lastIndex++;
      swap(list[lastIndex], list[index]);
    }
  }
  swap(list[lastIndex + 1], list[last]);
  return (lastIndex + 1);
}

void quickSort(Vector <int> &list, int first, int last) {
	
	for (int times = 0; times < 1; times++) {
		
		// if the list isn't sorted
		if (first < last) {
			
			// set the index that will be used to divide the vector 
			int pivot = part(list, first, last);
	    
			//Recursion function
			// make a sub vector with the values above pivot
			quickSort(list, pivot+1, last);
			// make a sub vector with the values lower pivot
			quickSort(list, first, pivot-1);
		}
	}
}

void printSortedVector(Vector <int> list) {
	
	cout << "The vector sorted is: ";
	
	// for each index in the list
	for (int index = 0; index < list.size(); index++) {
		
		// print it
		cout << list[index] << ", ";
	}
}

int main() {
	
	Vector <int> list;
	
	// add values to the vector
	list.add(67);
	list.add(34);
	list.add(15);
	list.add(98);
	list.add(134);
	list.add(21);
	list.add(56);
	list.add(67);
	list.add(76);
	list.add(10);
	list.add(1);
	list.add(5);
	list.add(12);
	list.add(1000);
	list.add(1345);
	list.add(2112);
	list.add(5634);
	list.add(6712);
	list.add(7623);
	list.add(1340);

	int first = 0;

	int last = list.size();
	
	// recursive sort the list of numbers
	quickSort(list, first, last -1);
	
	// print the sorted vector
	printSortedVector(list);

	return 0;
}