/* File: boggle.cpp
 * ----------------
 * Your name here!
 */
 
#include "genlib.h"
#include "simpio.h"
#include <iostream>
#include "extgraph.h"
#include "lexicon.h"
#include "gboggle.h"
#include "Random.h"
#include "Vector.h"
#include "Grid.h"
#include "Map.h"
#include "Set.h"

void GiveInstructions()
{
    cout << endl << "The boggle board is a grid onto which I will randomly distribute " 
	 << "cubes. These 6-sided cubes have letters rather than numbers on the faces, " 
	 << "creating a grid of letters on which you try to form words. You go first, " 
	 << "entering all the words you can find that are formed by tracing adjoining " 
	 << "letters. Two letters adjoin if they are next to each other horizontally, " 
	 << "vertically, or diagonally. A letter can only be used once in the word. Words "
	 << "must be at least 4 letters long and can only be counted once. You score points "
	 << "based on word length: a 4-letter word is worth 1 point, 5-letters earn 2 "
	 << "points, and so on. After your puny brain is exhausted, I, the super computer, "
	 << "will find all the remaining words and double or triple your paltry score." << endl;
	
    cout << "\nHit return when you're ready...";
    GetLine();
}

static void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle. " 
	 << "The good news is that you might improve your vocabulary a bit.  The "
	 << "bad news is that you're probably going to lose miserably to this little "
	 << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

// draw the table and label all cubes 
Grid<char> setupTable(int size, string letters) {
    
	// draw the table with the correct size using the function DrawBoard() from gboogle.h
    DrawBoard(size, size);
	
	// create a grid with the size passed
    Grid<char> table(size,size);
	
	// for each row
    for(int indexOfRow = 0; indexOfRow < size; indexOfRow++){
		
		// for each column
        for(int indexOfColumn = 0; indexOfColumn < size; indexOfColumn++){
			
			// add the letter for the column using the function LabelCube() from gboogle.h
			LabelCube(indexOfRow,indexOfColumn,letters[indexOfRow * size + indexOfColumn]);
            
			// add the letter to the table
			table.setAt(indexOfRow,indexOfColumn,letters[indexOfRow *s ize + indexOfColumn]);
        }
    }
    return table;
}

// turn every char in the sequence to upper case
string upperCase(string letters) {
	
	// for each char
	for (int index = 0; index < letters.length(); index++) {
		
		// turn the char to upper case
		letters[index] = toupper(letters[index]);
	}
	
	return letters;
}

void mountGame(int boardSize, string letters) {
	
	//Lexicon lex("lexicon.dat");
	
	// draw the table
	Grid <char> table = setupTable(boardSize, letters);
	

}

string GenerateRandomRoll(int size){
	
	// create a vector to store the letters
	Vector<string> boggleCubes(25);

	string StandardCubes[16]  = 
{"AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
 "DISTTY", "EEGHNW", "EEINSU", "EHRTVW", "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};
 
	string BigBoggleCubes[25]  = 
{"AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM", "AEEGMU", "AEGMNN", "AFIRSY", 
"BJKQXZ", "CCNSTW", "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT", "DHHLOR", 
"DHLNOR", "EIIITT", "EMOTTT", "ENSSSU", "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};
	
	// if the standard option was selected
    if (size == 4) {
		
		// for each standard char sequences
        for(int index = 0; index < 16; index++){
			
			// add the the standard char sequences to the vector
			boggleCubes.add(StandardCubes[index]);
        }
    } 
	
	// if the big table option was selected
	else {
        
		// for each char
		for(int index = 0; index < 25; index++){
            
			// add the char to the vector
			boggleCubes.add(BigBoggleCubes[index]);
        }
    }

    // chooses a random side from each cube and adds it to the output string.
    string letters;

    for(int i = 0; i < size*size; i++){

        letters += boggleCubes[i][RandomInteger(0,5)];
    }
    return letters;
}

void playGame(){
    
	// reapeat the game until the player enter an empty string ("")
	while(true){
		
		// set the table size
		int tableSize = 0;
		
        cout << "Enter (1) for a 4x4 game, or (2) for a 5x5 game." <<endl;
		
		// get the choice from the user
		int choice = GetInteger();
        
		// if the user choosed the 4x4
		if(choice == 1) {
			
			// set the table size to 4
			tableSize = 4;
		}
		
		// if the user choosed the 5x5
		else {
			
			// set the table size to 5
			tableSize = 5;
		}
        
		cout << "Enter (1) for a random board. Or a sequence of 25 letters for a custom board." << endl;
		
		// get the sequence of char from the user or the confirmation for random mode
		string tableConfig = GetLine();
           
		// if the user selected random
		if(tableConfig == "1"){
            
			// mount the game with random chars
			mountGame(tableSize, GenerateRandomRoll(tableSize));
        } 
		// if the user entered a sequence of chars
		else if(tableConfig.length() == tableSize * tableSize) {
			
			// mount the game with the entered chars
			mountGame(tableSize, upperCase(tableConfig));
        }
		
		// if the user enterd "" empty string stop the game
		else if (tableConfig == "") {
			
			break;
		}
    }
}


int main()
{
	SetWindowSize(9, 5);
	InitGraphics();
	Welcome();
	GiveInstructions();
	playGame();
	return 0;
}
