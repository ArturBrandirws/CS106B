#include <iostream>

class PriorityQueue {
private:
	// Maximum size of the priority queue
    static const int MAX_SIZE = 100; 
    // Array to store elements
	int elements[MAX_SIZE]; 
	// Current size of the priority queue
    int size; 

public:
    // Constructor
    PriorityQueue() : size(0) {}
	// add the value to the array and reorganize the array to maintain the heap
    void enqueue(int value) {
        // Add the value to the array
        elements[size++] = value;
        // Perform bubble-up operation to maintain the heap
        bubbleUp(size - 1);
    }
	
	// remove the larger value from the array and reorganize the array to maintain the heap
    int dequeue() {
        if (size == 0) {
            std::cerr << "Queue is empty!" << std::endl;
            return -1; // Return a default value indicating failure
        }
        // Retrieve the root element
        int frontValue = elements[0];
        // Replace the root element with the last element
        elements[0] = elements[--size];
        // Perform bubble-down operation to maintain the heap property
        bubbleDown(0);
        return frontValue;
    }
	
	// print the heap 
    void printHeap() {
        std::cout << "Priority Queue: ";
		// for each node
        for (int node = 0; node < size; ++node) {
			// print the node
            std::cout << elements[node] << " ";
        }
        std::cout << std::endl;
    }

private:
	// function to maintain the heap property by bubbling up when a new value is inserted
    void bubbleUp(int index) {
        
		// while the element isn't in the correct position
		while (index > 0 && elements[index] > elements[parent(index)]) {

            // Swap the element with its parent
            int temp = elements[index];
            elements[index] = elements[parent(index)];
            elements[parent(index)] = temp;
            index = parent(index);
        }
    }

	// function to maintain the heap property by bubbling down when a new value is inserted
    void bubbleDown(int index) {
        while (index < size) {
			
			// get the leftChild index
            int leftChild = leftChildIndex(index);

			// get the rightChild index
            int rightChild = rightChildIndex(index);
            int maxIndex = index;

            if (leftChild < size && elements[leftChild] > elements[maxIndex]) {
                maxIndex = leftChild;
            }
            if (rightChild < size && elements[rightChild] > elements[maxIndex]) {
                maxIndex = rightChild;
            }
            if (maxIndex != index) {
                
				// Swap the element with its larger child
                int temp = elements[index];
                elements[index] = elements[maxIndex];
                elements[maxIndex] = temp;
                index = maxIndex;
            } else {
                break;
            }
        }
    }

    // Helper function to get the index of the parent of the node
    int parent(int index) {
        return (index - 1) / 2;
    }

    // Helper function to get the index of the left child of the node
    int leftChildIndex(int index) {
        return 2 * index + 1;
    }

    // Helper function to get the index of the right child of the node
    int rightChildIndex(int index) {
        return 2 * index + 2;
    }
};
