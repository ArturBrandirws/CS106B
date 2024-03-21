#include "vector.h"
#include "genlib.h"
#include "simpio.h"
#include <iostream>

// recursive function
bool Solvable(int start, Vector<int> &squares, Vector<bool> &visited) {
    
	// Mark the current square as visited
    visited[start] = true;
	
	// create a int with the value of the current square
    int squareValue = squares[start];
	
	// If reached the goal square, puzzle is solvable
    if (squareValue == 0) {
        
        return true;
    }

    // Move left

	// create an int with the index of the left possible square
    int leftSquare = start - squareValue;

	// if the left square is inside the vector range and havent been visited
    if (leftSquare >= 0 && !visited[leftSquare]) {

		// if solvable return true
        if (Solvable(leftSquare, squares, visited)) {
            return true;
        }
    }

    // Move right

	// create an int with the index of the right possible square
    int rightSquare = start + squareValue;
	
	// if the right square is inside the vector range and havent been visited
    if (rightSquare < squares.size() && !visited[rightSquare]) {
		
		// if solvable return true
        if (Solvable(rightSquare, squares, visited)) {
            return true;
        }
    }

    // If no solution found from this square
    return false;
}

int main() {
	
	// create a vector to store the squares values
	Vector <int> squares;
	
	// create a vector to store visited squares
	Vector <bool> visitedSquares;
	
	// add the values to the squares
	squares.add(3);
	squares.add(6);
	squares.add(4);
	squares.add(1);
	squares.add(3);
	squares.add(4);
	squares.add(2);
	squares.add(5);
	squares.add(3);
	squares.add(0);
	
	// initializes the size of the visited squares vector with falses
	for (int index = 0; index < squares.size(); index++) {
		
		// add false to the index
		visitedSquares.add(false);
	}
	
	// define the start position
	int start = 0;
	
	// get the true or false return from the solvable checker
	bool possible = Solvable(start, squares, visitedSquares);
	
	// print if is possible
	if (possible == true) {
		
		cout << "This square configuratuion is possible" << endl;
	}
	
	// print if isn't possible
	else {
		
		cout << "This square configuratuion isn't possible" << endl;
	}
}