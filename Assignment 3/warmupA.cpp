#include <iostream>
#include "genlib.h" 
#include "simpio.h" 
#include <fstream>

void PrintInBinary(int number)
{
    if (number > 1)
		
		// print the first numbers
        PrintInBinary(number/2);
		
		// print the least number
		cout << (number % 2);	      
}

int main() {
	
	while (true){

		cout << endl;

		cout << "Enter a number to be printed in binary: ";

		int num = GetInteger();

		PrintInBinary(num);
	}
}