#include <iostream>
#include "genlib.h"
#include "simpio.h"
#include "vector.h"

int CountCriticalVotes(Vector<int> &blocks, int blockIndex, int currentBlockIndex, int candidateAVotes, int candidateBVotes) {
    
	if (currentBlockIndex == blocks.size()) {
        
		// Base case: reached the end of blocks
        return (candidateAVotes > candidateBVotes && blockIndex == 1) || (candidateBVotes > candidateAVotes && blockIndex == 0);
    }

    // Counting the number of critical votes recursively
    int count = 0;

    // Case where the current block is given to candidate A
    count += CountCriticalVotes(blocks, blockIndex, currentBlockIndex + 1, candidateAVotes + blocks[currentBlockIndex], candidateBVotes);

    // Case where the current block is given to candidate B
    count += CountCriticalVotes(blocks, blockIndex, currentBlockIndex + 1, candidateAVotes, candidateBVotes + blocks[currentBlockIndex]);

    return count;
}

int main() {
    
	// initializes the blocks vector
	Vector<int> blocks;
	
	// add values to the blocks
    blocks.add(4);
    blocks.add(2);
    blocks.add(7);
    blocks.add(4);


	// cout the amount of votes in the blocks
    for (int blockIndex = 0; blockIndex < blocks.size(); blockIndex++) {
        cout << "The block number " << blockIndex << " has " << blocks[blockIndex] << " votes" << endl;
    }
	
    cout << "Type the number of a block: ";

	// get the block index from the user
    int blockIndex = GetInteger();
	
	// get the count of how many times the vote was critical
    int count = CountCriticalVotes(blocks, blockIndex, 0, 0, 0);

    cout << "The block " << blockIndex << " is a critical vote " << count << " times." << endl;

    return 0;
}
