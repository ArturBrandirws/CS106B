#include <iostream>
#include "simpio.h"
#include "graphics.h"

// recursive function
void DrawSmallerLines(double startX, double startY, double endX, double endY) {
        
		// define new start X location
		double nextPenLocation = startX + endX / 2;
		
		MovePen(nextPenLocation, startY);
        
		// draw 1/4 lines
		DrawLine(0, endY);
        
		if (endX > 0.1) {
			
			// define new end X location
			double newEndX = endX / 2;
			
			// define new end Y location
			double newEndY = endY / 2;
            
			// draw 1/8 lines
			DrawSmallerLines(startX, startY, newEndX, newEndY);
			
			// draw 1/16 lines
			DrawSmallerLines(nextPenLocation, startY, newEndX, newEndY);
        }
}

// wrapper function
void DrawBiggerLine(double startX, double startY, double endX, double endY) {

	MovePen(startX, startY);
    
	// draw inche line
	DrawLine(endX,0);
    
	// draw 1/4 lines, 1/8 lines and 1/16 lines
	DrawSmallerLines(startX, startY, endX, endY);
}

int main () {
	
	// init the graphics window
	InitGraphics();
	
	// define the start X location
	double startX = 1.0;
	
	// define the start Y location
	double startY = 1.5;
	
	// define the end X location
	double endX = 8.0;
	
	// define the end Y location
	double endY = 1.0;
	
	// draw the base line
	DrawBiggerLine(startX, startY, endX, endY);

    return 0;
}
