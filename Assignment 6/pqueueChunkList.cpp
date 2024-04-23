#include <iostream>

// Define the maximum number of elements per block
const int maxNumBlock = 4;

// Define the structure of a block
struct Block {
    
    // Array to store elements
    int elements[maxNumBlock]; 
    
    // Number of elements currently in the block
    int numElements;  
    
    // Pointer to the next block         
    Block* next;               

    // Constructor to initialize the block
    Block() : numElements(0), next(NULL) {}
};

// Define the priority queue class
class PriorityQueue {
private:
    
    // Points to the first block of the chunklist
    Block* head; 

public:
    
    // Constructor to initialize the priority queue
    PriorityQueue() : head(NULL) {}

    // Function to enqueue an element into the priority queue
    void enqueue(int value) {

        // If the priority queue is empty or the first block is full, create a new block
        if (head == NULL || head->numElements == maxNumBlock) {
            Block* newBlock = new Block();
            newBlock->next = head;
            head = newBlock;
        }

        // If the value is smaller than the smallest element in the head block, create a new block
        if (head->numElements > 0 && value < head->elements[head->numElements - 1]) {
            Block* newBlock = new Block();
            newBlock->next = head;
            head = newBlock;
        }

        // Insert the element into the appropriate position in the block (descending order)
        int i = head->numElements;
        while (i > 0 && head->elements[i - 1] < value) {
            head->elements[i] = head->elements[i - 1];
            i--;
        }
        head->elements[i] = value;
        head->numElements++;
    }

    // Function to dequeue an element from the priority queue
    int dequeue() {
    
        // Remove and return the first element from the first block
        int value = head->elements[0];

        // Shift elements to the left to fill the empty space
        for (int i = 1; i < head->numElements; ++i) {
            head->elements[i - 1] = head->elements[i];
        }

        // Decrement the number of elements in the block
        --head->numElements;

        // If the block becomes empty after dequeue, remove it
        if (head->numElements == 0) {
            Block* temp = head;
            head = head->next;
            delete temp;
        }

        return value;
    }

    // Function to print the contents of the priority queue
    void print() {
        Block* current = head;
        std::cout << "The Blocks are: " << std::endl;
        while (current != NULL) {
            std::cout << "Block: ";
            for (int i = 0; i < current->numElements; i++) {
                std::cout << current->elements[i] << " ";
            }
            std::cout << std::endl;
            current = current->next;
        }
    }
};

// Main function
int main() {
    // Create a priority queue object
    PriorityQueue pq;

    // Enqueue elements
    pq.enqueue(5);
    pq.enqueue(14);
    pq.enqueue(14);
    pq.enqueue(17);
    pq.enqueue(22);
    pq.enqueue(29);

    // Print the priority queue
    pq.print();

    // Dequeue elements
    int dequeued = pq.dequeue();
    std::cout << std::endl;
    std::cout << "Dequeued Element: " << dequeued << std::endl;

    // Print the priority queue after dequeue
    std::cout << std::endl;
    std::cout << "Priority Queue after Dequeue: ";
    pq.print();

    return 0;
}
