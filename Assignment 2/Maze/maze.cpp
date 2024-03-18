#include <iostream>
#include <cstdlib>
#include <ctime>
#include "maze.h"
#include "stack.h"
#include "queue.h"

using namespace std;

// Declare a constant for the number of directions (4 directions)
const int kNumDirs = 4;
// Declare an array representing changes in x-coordinate for each direction
const int kDx[kNumDirs] = {0, 1, 0, -1};
// Declare an array representing changes in y-coordinate for each direction
const int kDy[kNumDirs] = {-1, 0, 1, 0};

// Define a function to generate the maze
void generateMaze(Maze& maze) {
   
	// Get the number of rows in the maze
    int numRows = maze.numRows();
    
	// Get the number of columns in the maze
    int numCols = maze.numCols();

    // allocate memory to track included cells
    bool** included = new bool*[numRows];
    
	// Loop through each row
    for (int i = 0; i < numRows; i++) {
       
		// Allocate memory for each row
        included[i] = new bool[numCols];
       
		// Loop through each column
        for (int j = 0; j < numCols; j++) {
            
			// Initialize each cell as excluded
            included[i][j] = false;
        }
    }

    // Choose a random starting row
    int startRow = rand() % numRows;
   
	// Choose a random starting column
    int startCol = rand() % numCols;
    
	// Define a point representing the current cell
    pointT currentPoint = {startRow, startCol};
    
	// Mark the starting cell as included
    included[startRow][startCol] = true;

    // Repeat until all cells are included
    while (true) {
        
		// Choose a random direction
        int dir = rand() % kNumDirs;
        
		// Calculate the row of the neighbor cell
        int nextRow = currentPoint.row + kDy[dir];
        
		// Calculate the column of the neighbor cell
        int nextCol = currentPoint.col + kDx[dir];

        // Check if the neighbor cell is within bounds and not included
        if (nextRow >= 0 && nextRow < numRows && nextCol >= 0 && nextCol < numCols &&
            !included[nextRow][nextCol]) {
           
			// Remove the wall between the current cell and the neighbor cell
            maze.setWall(currentPoint, {nextRow, nextCol}, false);
            
			// Mark the neighbor cell as included
            included[nextRow][nextCol] = true;
        }

        // Update the current cell
        currentPoint = {nextRow, nextCol};

        // Flag to check if all cells are included
        bool allIncluded = true;
        
		// Loop through each row
        for (int row = 0; row < numRows; row++) {
            
			// Loop through each column
            for (int col = 0; col < numCols; col++) {
                
				// If any cell is not included
                if (!included[row][col]) {
                    
					// Update the flag
                    allIncluded = false;
                    break;
                }
            }
            
			// If any cell is not included, exit the loop
            if (!allIncluded) break;
        }
        
		// If all cells are included, exit the loop
        if (allIncluded) break;
    }

    // Loop to free dynamically allocated memory
    for (int i = 0; i < numRows; i++) {
        
		// Delete memory for each row
        delete[] included[i];
    }
    
	// Delete memory for the array of rows
    delete[] included;
}

// Define a function to solve the maze using breadth-first search
Stack<pointT> solveMaze(Maze& maze) {
    
	// Get the number of rows in the maze
    int numRows = maze.numRows();
    
	// Get the number of columns in the maze
    int numCols = maze.numCols();

    // Create a queue of paths
    Queue<Stack<pointT>> queue;

    // Create a stack representing the starting path
    Stack<pointT> startPath;
    
	// Define the starting point at the lower-left corner
    pointT start = {0, 0};
    
	// Push the starting point onto the stack
    startPath.push(start);
    
	// Enqueue the starting path
    queue.enqueue(startPath);

    // Repeat until the queue is empty
    while (!queue.isEmpty()) {
       
		// Dequeue a path from the queue
        Stack<pointT> currentPath = queue.dequeue();
        
		// Get the last point in the path
        pointT lastPoint = currentPath.peek();

        // If the last point is the goal
        if (lastPoint.row == numRows - 1 && lastPoint.col == numCols - 1) {
            
			// Return the path
            return currentPath;
        }

        // Loop through each direction
        for (int dir = 0; dir < kNumDirs; dir++) {
            
			// Calculate the row of the neighbor cell
            int nextRow = lastPoint.row + kDy[dir];
            
			// Calculate the column of the neighbor cell
            int nextCol = lastPoint.col + kDx[dir];

            // Check if the neighbor cell is within bounds and not a wall
            if (nextRow >= 0 && nextRow < numRows && nextCol >= 0 && nextCol < numCols &&
                !maze.isWall(lastPoint, {nextRow, nextCol})) {
                
				// Create a copy of the current path
                Stack<pointT> newPath = currentPath;
                
				// Extend the path by adding the neighbor cell
                newPath.push({nextRow, nextCol});
                
				// Enqueue the new path
                queue.enqueue(newPath);
            }
        }
    }

    // If no path is found, return an empty stack
    return Stack<pointT>();
}

// Define a function to mark the solution path in the maze
void markSolution(Maze& maze, Stack<pointT>& solution) {
    
	// Repeat until the solution stack is empty
    while (!solution.isEmpty()) {
       
		// Pop a point from the solution stack
        pointT p = solution.pop();
        
		// Mark the point as part of the solution path
        maze.drawMark(p, "Red");
    }
}

// Define the main function
int main() {
    
	// Define the number of rows in the maze
    const int kNumRows = 10;
    
	// Define the number of columns in the maze
    const int kNumCols = 10;

    // Create a maze object with the specified dimensions
    Maze maze(kNumRows, kNumCols, true);
   
	// Generate the maze
    generateMaze(maze);
    
	// Draw the maze
    maze.draw();

    // Find the solution path for the maze
    Stack<pointT> solution = solveMaze(maze);
    
	// Mark the solution path in the maze
    markSolution(maze, solution);

    // Exit the program
    return 0;
}