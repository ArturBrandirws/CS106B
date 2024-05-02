// pathfinder.cpp
 
#include "genlib.h"
#include "simpio.h"
#include <fstream>
#include "map.h"
#include "stack.h"
#include "set.h"
#include "extgraph.h"
#include <iostream>
#include <string>
#include <cmath>
#include "Vector.h"
#include "GraphicsLib.h"
#include "pqueue.h"
#include "cmpfn.h"

/* Constants
 * --------
 * A few program-wide constants concerning the graphical display.
 */
const double CircleRadius =.05;     	// the radius of a node
const int LabelFontSize = 9;          // for node name labels


/* Type: coordT
 * ------------
 * Just a simple struct to handle a pair of x,y coordinate values.
 */
struct coordT {
	double x, y;
};

/* Function: DrawFilledCircleWithLabel
 * Usage:  DrawFilledCircleWithLabel(center, "Green", "You are here");
 * -------------------------------------------------------------------
 * Uses facilities from extgraph to draw a circle filled with
 * color specified. The circle is centered at the given coord has the
 * specified radius.  A label is drawn to the right of the circle.
 * You can leave off the last argument if no label is desired.
 */
void DrawFilledCircleWithLabel(coordT center, string color, string label)
{
	MovePen(center.x + CircleRadius, center.y);
	SetPenColor(color);
	StartFilledRegion(1.0);
	DrawArc(CircleRadius, 0, 360);
	EndFilledRegion();
	if (!label.empty()) {
		MovePen(center.x + CircleRadius, center.y);
		SetFont("Helvetica");
		SetPointSize(LabelFontSize);
		DrawTextString(label);
	}
}

/* Function: DrawLineBetween
 * Usage:  DrawLineBetween(coord1, coord2, "Black");
 * -------------------------------------------------
 * Uses facilities from extgraph to draw a line of the
 * specified color between the two given coordinates.
 */
void DrawLineBetween(coordT start, coordT end, string color)
{
	SetPenColor(color);
	MovePen(start.x, start.y);
	DrawLine(end.x - start.x, end.y - start.y);
}


/* Function: GetMouseClick
 * Usage:  loc = GetMouseClick();
 * ------------------------------
 * Waits for the user to click somewhere on the graphics window
 * and returns the coordinate of where the click took place.
 */
coordT GetMouseClick()
{
	coordT where;
	WaitForMouseDown();
	WaitForMouseUp();
	where.x = GetMouseX();
	where.y = GetMouseY();
	return where;
}

/* Function: WithinDistance
 * Usage:  if (WithinDistance(click, pt))...
 * -----------------------------------------
 * Returns true if the Cartesian distance between the two coordinates
 * is <= the specified distance, false otherwise. If not specified,
 * the distance is assumed to be size of the node diameter on screen.
 */
bool WithinDistance(coordT pt1, coordT pt2, double maxDistance)
{
	double dx = pt2.x - pt1.x;
	double dy = pt2.y - pt1.y;
	double distance = sqrt(dx*dx + dy*dy);
	return (distance <= maxDistance);
}

// a line that connects 2 nodes
struct arcT {
    nodeT * end1, * end2;
    double distance;
};

struct Node {
    std::string name;
    coordT location;
    Set<std::string> connections;
    Vector<arcT*> arcs;
};

// Function to calculate the total distance of a path represented by a stack of arcs
double calculatePathDistance(Stack<arcT*> path) {
    // Initialize total distance accumulator
    double totalDistance = 0;
    // Loop until the path stack is empty
    while (!path.isEmpty()) {
        // Add the distance of the top arc in the stack to the total distance and remove the arc from the stack
        totalDistance += path.pop()->distance;
    }
    // Return the accumulated total distance of the path
    return totalDistance;
}

// Function to compare the total distances of two paths and determine their order
int comparePaths(Stack<arcT*> path1, Stack<arcT*> path2) {
    // Calculate the total distance of path1 and path2 using the calculatePathDistance function
    double distance1 = calculatePathDistance(path1);
    double distance2 = calculatePathDistance(path2);

    // Compare the total distances of the two paths
    if (distance1 < distance2)
        // If distance1 is less than distance2, return 1 indicating path1 should come before path2
        return 1;
    else if (distance1 > distance2)
        // If distance1 is greater than distance2, return -1 indicating path2 should come before path1
        return -1;
    else
        // If distances are equal, return 0 indicating both paths are of the same length
        return 0;
}

// Function to compare two arc pointers based on their distances
int compareArcPointers(arcT* a, arcT* b) {
    // Compare the distances of arc a and arc b
    if (a->distance < b->distance)
        // If distance of a is less than distance of b, return 1
        return 1;
    else if (a->distance > b->distance)
        // If distance of a is greater than distance of b, return -1
        return -1;
    else
        // If distances are equal, return 0
        return 0;
}

// Function to extract the next word from a given string starting at position 'pos'
std::string extractNextWord(int& pos, std::string line) {
    // Initialize an empty string to store the extracted word
    std::string result = "";

    // Loop through the characters in the string 'line' starting from position 'pos'
    while (pos < line.length()) {
        // Extract the next character from 'line' at position 'pos'
        std::string nextChar = line.substr(pos++, 1);

        // Check if the extracted character is a space (indicating end of the word)
        if (nextChar == " ")
			// If space is found, exit the loop
            break; 

        // Append the extracted character to the result string
        result = result + nextChar;
    }

    // Return the extracted word
    return result;
}

std::string loadMapFromFile(std::string fileName, Map<Node*>& nodes, Vector<arcT*>& arcs) {
	// Variable to store the background map image filename
    std::string mapImage; 
	// Open the file for reading
    std::ifstream fileTyped(fileName); 
	// Check if the file is successfully opened
    if (fileTyped.is_open()) { 
		// Move graphics pen to the starting position
        MovePen(0, 0); 
		// Read the first line (background image filename)
        getline(fileTyped, mapImage); 
		// Draw the named picture (background image)
        DrawNamedPicture(mapImage); 
		// Variable to store each line of the file
        std::string line; 
		// Read the next line
        getline(fileTyped, line); 
		
		// Read lines until reaching the start of the arcs section
        while (getline(fileTyped, line) && line != "ARCS") { 
			// Create a new node object
            Node* nextNode = new Node; 
			// Initialize index for extracting words from the line
            int i = 0; 
			// Extract node name from the line
            nextNode->name = extractNextWord(i, line); 
			// Extract x-coordinate of node location
            nextNode->location.x = atof(extractNextWord(i, line).c_str()); 
			// Extract y-coordinate of node location
            nextNode->location.y = atof(extractNextWord(i, line).c_str()); 
			// Add node name to its own connections (self-connected)
            nextNode->connections.add(nextNode->name); 
			// Add node to the map using its name as the key
            nodes[nextNode->name] = nextNode; 
        }
		
        // Read lines for arcs section
        while (getline(fileTyped, line)) {
            // Create a new arc object
            arcT* nextArc = new arcT;
            // Initialize index for extracting words from the line
            int i = 0;
            // Get the first endpoint node of the arc
            nextArc->end1 = nodes[extractNextWord(i, line)];
            // Get the second endpoint node of the arc
            nextArc->end2 = nodes[extractNextWord(i, line)];
            // Get the distance value of the arc
            nextArc->distance = atof(extractNextWord(i, line).c_str());
            // Add the arc to the arcs list of the first endpoint node
            nextArc->end1->arcs.add(nextArc);
            // Add the arc to the arcs list of the second endpoint node
            nextArc->end2->arcs.add(nextArc);
            // Add the arc to the vector of arcs
            arcs.add(nextArc);
        }
        // Close the file after reading
        fileTyped.close();
    } 
	else {
        // Display error message if file opening fails
        std::cerr << "Error: Unable to open file." << std::endl;
    }
    // Return the filename of the background map image
    return mapImage;
}


// Function to prompt the user for a choice and validate the input
std::string getUserChoice() {
    // Display options to the user
    std::cout << std::endl << std::endl << "Your options are:" << std::endl
              << "(1) Choose a new graph data file" << std::endl
              << "(2) Find shortest path using Dijkstra's algorithm" << std::endl
              << "(3) Compute the minimal spanning tree using Kruskal's algorithm" << std::endl
              << "(4) Quit" << std::endl
              << "Enter choice: ";

    // Variable to store the user's choice
    std::string choice;
    
    // Read user input
    std::getline(std::cin, choice);
    
    // Validate the user's choice
    while (choice != "1" && choice != "2" && choice != "3" && choice != "4") {
        // Display error message for invalid choice
        std::cout << "Invalid choice, please try again: ";
        // Prompt user to enter a valid choice
        std::getline(std::cin, choice);
    }
    
    // Return the valid user choice
    return choice;
}

void displayNodes(Map<Node*>& nodes) {
    // Iterate over each element in the nodes map using an index
    Map<Node*>::Iterator iter = nodes.iterator();
    while (iter.hasNext()) {
		// Get the node name (key) at the current iterator position
        std::string nodeName = iter.next();  
		// Get the node pointer (value) associated with the key
        Node* nodePtr = nodes[nodeName];     

        // Draw a filled circle with a label representing the node on the graphics window
        // using the node's location (nodePtr->location), color ("black"), and name (nodeName)
        DrawFilledCircleWithLabel(nodePtr->location, "black", nodeName);
    }
}

// Function to display arcs on the graphics window
void displayArcs(Vector<arcT*>& arcs, std::string color) {
    // Loop through each arc in the arcs vector using traditional for loop with index
    for (int i = 0; i < arcs.size(); ++i) {
        // Get the current arc pointer from the vector using the index
        arcT* arc = arcs[i];
        
        // Draw a line between the two endpoints of the current arc using specified color
        DrawLineBetween(arc->end1->location, arc->end2->location, color);
    }
}

// Function to get a node from a mouse click within a given map of nodes
Node* clickNode(Map<Node*>& nodes) {
    // Loop indefinitely until a node is clicked
    while (true) {
        // Get the coordinates of the mouse click
        coordT clicked = GetMouseClick();

        // Iterate over each node in the map using keys
        for (int i = 0; i < nodes.keys().size(); ++i) {
            std::string nodeName = nodes.keys().get(i);
            Node* node = nodes[nodeName];

            // Check if the mouse click is within the distance of the node's location
            if (WithinDistance(node->location, clicked)) {
                // Return the node if the mouse click is near the node's location
                return node;
            }
        }
    }
}

void displayPath(Stack<arcT*>& arcs) {
    // Initialize total distance accumulator
    double totalDistance = 0;

    // Create a stack to hold reversed arcs
    Stack<arcT*> reversedArcs;

    // Reverse the arcs stack and push into reversedArcs stack
    while (!arcs.isEmpty()) {
        reversedArcs.push(arcs.pop());
    }
    // Display each arc in reversed order
    while (!reversedArcs.isEmpty()) {
        // Get the next arc from reversedArcs stack
        arcT* arc = reversedArcs.pop();

        // Draw filled circles at the start and end nodes of the arc in red color
        DrawFilledCircleWithLabel(arc->end1->location, "red", arc->end1->name);
        DrawFilledCircleWithLabel(arc->end2->location, "red", arc->end2->name);

        // Draw a line between the start and end nodes of the arc in red color
        DrawLineBetween(arc->end1->location, arc->end2->location, "red");

        // Add the arc's distance to the total distance
        totalDistance += arc->distance;
    }
    // Output the total distance of the path to the console
    std::cout << "Total distance is " << totalDistance << "." << std::endl;
}


void enqueueAdjacentArcs(Node* node, Stack<arcT*>& path, PQueue<Stack<arcT*>>& paths) {
    // Iterate over each arc connected 
	for (int i = 0; i < node->arcs.size(); i++) {
		// Get the current arc pointer from the arcs vector using the index 'i'
		auto arc = node->arcs[i];
		// Add the current arc to the path stack
		path.push(arc);
		// Enqueue the current path (with the added arc) into the priority queue of paths
		paths.enqueue(path, comparePaths);
		// Remove the current arc from the path stack (restoring the original state of 'path')
		path.pop();
	}
}

void findShortestPath(Node* startNode, Node* endNode) {
    // Create a stack to store the current path being explored
    Stack<arcT*> path;

    // Create a priority queue to manage paths based on their total distance
    PQueue<Stack<arcT*>> paths;

    // Create a set to keep track of nodes that are included in the shortest path
    Set<Node*> includedNodes;

    // Add the start node to the set of included nodes
    includedNodes.add(startNode);

    // Enqueue arcs adjacent to the start node to begin exploring paths
    enqueueAdjacentArcs(startNode, path, paths);

    // Main loop to find the shortest path
    while (true) {
        // Dequeue the path with the maximum priority (shortest distance) from the priority queue
        path = paths.dequeue(comparePaths);

        // Check if the path reaches the end node
        if (path.peek()->end1 == endNode || path.peek()->end2 == endNode) {
            // Exit the loop if the end node is reached
            break;
        }

        // Determine the next node to explore in the path
        Node* nextNode = path.peek()->end1;
        if (includedNodes.contains(nextNode)) {
            // If the next node is already included in the path, explore the other end of the arc
            nextNode = path.peek()->end2;
        }

        // If the next node has not been included in the path yet
        if (!includedNodes.contains(nextNode)) {
            // Add the next node to the set of included nodes
            includedNodes.add(nextNode);
            // Enqueue arcs adjacent to the next node to continue exploring paths
            enqueueAdjacentArcs(nextNode, path, paths);
        }
    }
    // Display the shortest path on the graphics window
    displayPath(path);
}

void findMinimalSpanningTree(Map<Node*>& nodes, Vector<arcT*>& arcs, std::string mapImage) {
    // Set to keep track of included nodes in the minimal spanning tree
    Set<std::string> includedNodes;
    // Vector to store included arcs in the minimal spanning tree
    Vector<arcT*> includedArcs;
    // Priority queue to store arcs sorted by distance
    PQueue<arcT*> sortedArcs;
    // Variable to count the number of connections in the minimal spanning tree
    int numConnections = 0;
    // Variable to calculate the total network distance of the minimal spanning tree
    double totalDistance = 0.0;

    // Enqueue all arcs into the priority queue and sort them by distance
    for (int arc = 0; arc < includedArcs.size(); arc++) {
        sortedArcs.enqueue(arc, compareArcPointers);
    }
    // Main loop to construct the minimal spanning tree
    while (true) {
        // Dequeue the arc with the maximum distance
        arcT* arc = sortedArcs.dequeueMax(compareArcPointers);

        // Check if adding this arc would create a cycle
        if (!arc->end1->connections.contains(arc->end2->name)) {
            // Union the connection sets of end1 and end2 nodes
            arc->end1->connections.unionWith(arc->end2->connections);
            // Update the connection sets of nodes based on the union
            for (const auto& name : arc->end1->connections) {
                nodes[name]->connections = arc->end1->connections;
            }
            // Increment the number of connections in the minimal spanning tree
            numConnections++;
            // Add the arc to the included arcs vector
            includedArcs.add(arc);
            // Add the distance of the arc to the total network distance
            totalDistance += arc->distance;
        }

        // Check if all nodes are connected in the minimal spanning tree
        if (arc->end1->connections.size() == nodes.size()) {
            break; // Exit the loop if all nodes are connected
        }
    }
    // Display the included arcs in the minimal spanning tree on the graphics window
    displayArcs(includedArcs, "black");
    // Print information about the minimal spanning tree to the console
    std::cout << std::endl << "Minimal spanning tree requires " << numConnections << " connections." << std::endl
              << "To join " << nodes.size() << " nodes. Total network distance is " << totalDistance << "." << std::endl;
}

int main() {
    // Initialize graphics system
    InitializeGraphics();

    // Create map of nodes and vector of arcs
    Map<Node*> nodeMap;
    Vector<Arc*> arcVector;

    // Set window title
    SetWindowTitle("CS106 Pathfinder");

    // Display welcome message
    std::cout << "Welcome to the Graph Navigator!" << std::endl
              << "Explore the graph with Dijkstra's algorithm and find minimal spanning trees." << std::endl;
    // Variable to store the background map image
    std::string mapImage;

    // Main program loop
    while (true) {
        // Get user's choice
        std::string userChoice = GetUserChoice();

        // Load graph from a text file
        if (userChoice == "1") {
            std::cout << "Enter the filename: ";
            std::string filename = GetLine();
            // Load map from file and get the background image filename
            mapImage = LoadMapFromFile(filename, nodeMap, arcVector);
            // Display nodes on the graphics window
            DisplayNodes(nodeMap);
        } 
		// Find shortest path between two nodes
		else if (userChoice == "2") {
            // Check if the node map is empty
            if (nodeMap.isEmpty()) {
                std::cout << "Error: Please load a text file with at least two nodes first." << std::endl;
            } 
			else {
                // Display nodes and arcs on the graphics window
                DisplayNodes(nodeMap);

                // Prompt user to select the starting node
                std::cout << "Select the starting node by clicking." << std::endl;
                // Get the starting node from user click
                Node* startNode = clickNode(nodeMap);
                // Draw the starting node on the graphics window
                DrawNode(startNode->location, "red", startNode->name);

                // Prompt user to select the ending node
                std::cout << startNode->name << " selected. Please click on the ending node." << std::endl;
                // Get the ending node from user click
                Node* endNode = clickNode(nodeMap);
                // Draw the ending node on the graphics window
                DrawNode(endNode->location, "red", endNode->name);

                std::cout << endNode->name << " selected." << std::endl << std::endl;
                // Find and display the shortest path between the selected nodes
                FindShortestPath(startNode, endNode);
            }
		} 
		// Find minimal spanning tree
		else if (userChoice == "3") {
            // Check if the node map is empty
            if (nodeMap.isEmpty()) {
                std::cout << "Error: Please load a text file with at least two nodes first." << std::endl;
            } 
			else {
                // Clear the graphics window
                ClearScreen();
                // Display the background map image
                DisplayMap(mapImage);
                // Display nodes on the graphics window
                DisplayNodes(nodeMap);
                // Find and display the minimal spanning tree
                FindMinimalSpanningTree(nodeMap, arcVector, mapImage);
            }
        } 
		// Quit the program
		else if (userChoice == "4") {
            // Display farewell message
            std::cout << "Goodbye!" << std::endl;
            // Exit the main loop
            break;
        }
    }
    // Exit the program with success status
    return 0;
}