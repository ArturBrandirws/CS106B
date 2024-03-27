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

struct pointT { 
	int row; 
	int col; 
};

static void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle. " 
	 << "The good news is that you might improve your vocabulary a bit.  The "
	 << "bad news is that you're probably going to lose miserably to this little "
	 << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

// draw the table and label all cubes 
Grid <char> setupTable(int size, string letters) {
    
	// draw the table with the correct size using the function DrawBoard() from gboogle.h
    DrawBoard(size, size);
	
	// create a grid with the size passed
    Grid <char> table(size,size);
	
	// for each row
    for(int indexOfRow = 0; indexOfRow < size; indexOfRow++){
		
		// for each column
        for(int indexOfColumn = 0; indexOfColumn < size; indexOfColumn++){
			
			// add the letter for the column using the function LabelCube() from gboogle.h
			LabelCube(indexOfRow,indexOfColumn,letters[indexOfRow * size + indexOfColumn]);
            
			// add the letter to the table
			table.setAt(indexOfRow,indexOfColumn,letters[indexOfRow * size + indexOfColumn]);
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

Set <string> playerTurn(Grid <char> &table, Map<Vector<pointT>> &possibleWords) {
	
	// set a Set to store all words entered by the player
	Set <string> wordsEnteredByPlayer;
	
	cout << "Player's turn" << endl;
	
	// while the player dont exit his turn
	while(true) {

		cout << "Input word, or enter 'EXIT' to end turn: ";
		
		// get the word from the player
		string playerWord = GetLine();
		
		// turn the input into uppercase
		playerWord = upperCase(playerWord);
		
		// if the player ended the turn
		if (playerWord == "EXIT") {
			break;
		}
		
		// if the word has the minimum size, is possible in the table and the player haven't entered yet
		else if (playerWord.length() >= 4 && possibleWords.containsKey(playerWord) && !wordsEnteredByPlayer.contains(playerWord)) {
			
			cout << playerWord << " is a valid word!" << endl;
			
			// add the word in the list of the player
			wordsEnteredByPlayer.add(playerWord);
			
			// show in the graphic window the word entered
			RecordWordForPlayer(playerWord, Human);

		}

		// if the word hasn't the minimum size
		else if (playerWord.length() < 4) {
			
			cout << "The word must have at least 4 chars" << endl; 
		}
		
		// if the player enter the same word twice
		else if (wordsEnteredByPlayer.contains(playerWord)) {
			
			cout << "You've already entered this word" << endl;
		}
	}

	return wordsEnteredByPlayer;
}

bool ifPointIsContained(Vector<pointT> points, pointT point){
	
	// Iterating over each point in the vector
	for(int i = 0; i < points.size(); i++){
		
		// Checking if the current point matches the given point
		if(points[i].row == point.row && points[i].col == point.col){
			
			return true; 
		} 
	}
	
	return false; 
}

bool ifPointIsInBounds(pointT point, Grid<char> &table){
    
	// Checking if the row and column coordinates of the point are within the bounds of the grid
	if (point.row >= 0 && point.row < table.numRows() && point.col >= 0 && point.col < table.numCols() ){
            
		return true;
    }

    return false;
}

string makeWord(Vector<pointT> &points, Grid<char> &table){
    
	// String to store the constructed word
	string word; 
    
	for(int index = 0; index < points.size(); index++){
        
		// Constructing the word by adding the letters from the corresponding grid cells
		word += table(points[index].row,points[index].col);
    }

	return word; 
}

Vector<pointT>  FindNeighborsCells(Vector<pointT> &points, Grid<char> &table){
    
	// Vector to store neighboring points
	Vector<pointT> neighbors; 
    
	// Variable to store a single point
	pointT point; 
    
	// Iterating over neighboring cells of the last point in the sequence
	for(int row = -1; row < 2; row ++){

        for(int col = -1; col < 2; col++){
           
			// Calculating row coordinates for the neighboring cell
			point.row = points[points.size()-1].row + row;
			
			// Calculating column coordinates for the neighboring cell
            point.col = points[points.size()-1].col + col;
            
			// Adding the neighboring point to the vector
			neighbors.add(point);
        }
    }

	// Vector to store valid neighboring points
    Vector<pointT> neighborsValid; 
    
	// Checking validity of each neighboring point
	for(int index = 0; index < 9; index++){
        
		// Checking if the neighboring point is within bounds and not already visited
		if(ifPointIsInBounds(neighbors[index], table) && !ifPointIsContained(points, neighbors[index])){
           
			// Adding the valid neighboring point to the vector
			neighborsValid.add(neighbors[index]);
        }
    }
    
	return neighborsValid; 
}

void searchWords(Vector<pointT> points, Lexicon lex, Grid<char> &table, Map<Vector<pointT> > &possibleWords){
    
	// Constructing the word from the sequence of points
	string word = makeWord(points, table); 
    
	// Checking if the constructed word is valid and not already found
	if(lex.containsWord(word) && word.length() > 3 && !possibleWords.containsKey(word)){
		
		// Storing the word and its corresponding sequence of points
		possibleWords[word] = points; 
    }
   
	// Checking if the constructed word is a prefix of any valid word
	if(lex.containsPrefix(word)) {
		
		// Finding valid options for the next letter
		Vector<pointT> nextLetters = FindNeighborsCells(points, table); 
       
		// Recursively exploring valid options for the next letter
		for(int index = 0; index < nextLetters.size(); index++){
            
			// Adding the next letter option to the sequence
			points.add(nextLetters[index]); 
            
			// Recursive call
			searchWords(points, lex, table, possibleWords); 
            
			// Backtracking by removing the last added point
			points.removeAt(points.size()-1);
        }
    }
}

void findAllPossibleWords(Map<Vector<pointT>> &possibleWords, Grid <char> &table, Lexicon lex) {
	
	// Vector to store a sequence of points
	Vector<pointT> points; 
	
	// Variable to store a single point
	pointT point; 
   
	// Iterating over each cell in the Boggle table
	for(int row = 0; row < table.numRows(); row++){
        
		for(int col = 0; col < table.numCols(); col++){
			
			// Setting the row coordinate
			point.row = row; 
			
			// Setting the column coordinate
			point.col = col; 
            
			// Adding the current point to the sequence
			points.add(point); 
            
			// Finding words starting from the current point
			searchWords(points, lex, table, possibleWords);
            
			// Clearing the sequence for the next iteration
			points.clear(); 
        }
    }
}

void computerTurn(Grid <char> &table, Map<Vector<pointT>> &possibleWords, Set <string> &wordsEnteredByPlayer) {
	
	// create an iterator to iterate through the list of words
	Map<Vector<pointT> >::Iterator iter = possibleWords.iterator();
	
	// while the iterator has more tokens
    while (iter.hasNext()) {
		
		// create a string with the token
        string word = iter.next();
		
		// the token of the iterator isn't in the list of words entered by the player
        if(!wordsEnteredByPlayer.contains(word)){
			
			// print the word for the computer
			RecordWordForPlayer(word, Computer);
       }
    }
}

void mountGame(int tableSize, string letters) {
	
	// get all the words from the file
	Lexicon lex("lexicon.txt");
	
	// draw the table
	Grid <char> table = setupTable(tableSize, letters);
	
	// map to store words formed by the table
	Map<Vector<pointT>> possibleWords;
	
	// find all words formed by the table and save in the possible words
	findAllPossibleWords(possibleWords, table, lex);
	
	// get the Set with the words entered by the player and exec all interations of the player 
	Set <string> wordsEnteredByPlayer = playerTurn(table, possibleWords);
	
	// shows all possible words except the words entered by the player
	computerTurn(table, possibleWords, wordsEnteredByPlayer);
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
	
	// set a string for letters
    string letters;
	
	// shuffles the letters for diferent games
    for(int i = 0; i < size*size; i++){
		
		// add the cube
        letters += boggleCubes[i][RandomInteger(0, 5)];
    }
    return letters;
}

void playGame() {
    
	// reapeat the game until the player enter an empty string ("")
	while(true) {
		
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

// mai() function provided in the starter file
int main()
{	
	// set the window size
	SetWindowSize(9, 5);
	
	// initializes the graphics window
	InitGraphics();
	
	// cout all informations about the game
	Welcome();
	
	// cout all instructions about the game
	GiveInstructions();
	
	playGame();

	return 0;
}
