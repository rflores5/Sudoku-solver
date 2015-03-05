#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

template <typename T>
class Puzzle {

	public: 
		Puzzle();	//default constructor, reads in sudoku table from file
		void display();	//prints the sudoku table 
		int getboardsize(){
			return boardsize;	//determines the dimension of the table, either 9x9 or 4x4
		}
		void interactive();			//interactive mode for the user to play sudoku
		int playable(int r, int c, T n);	//determines if a number is playable, based on whether the number occurs in the same row,column,or minigrid
		int checkregion(int r, int c, T n);	//checks if the given number already exists in one of the nine minigrids
		int checkcompletion();			//checks if the game board is filled
		
		void oneoption(int i, int j);	//determines how many options there are for each cell, if only one option, then the cell is changed to that number			
		void singlepossibility();	//solves the sudoku board using the single possibility method
		void singleton();		//singleton method of solving sudoku puzzles
		void solver();			//combines single possibility and singleton method to solve medium and hard sudoku puzzles
		void update3Dvector();		//updates the possibilities vector, used after any changes to sudoku board occurs

	private: 
		vector<vector<T> > puzzle;			//2D vector array for the game board
		vector<vector<vector<T> > > possibilities;	//3D vector of possible values 
		int boardsize;					//dimension of board, either 2 or 9
};



template <typename T> 
Puzzle<T> :: Puzzle(){				//sudoku table is read from a file and placed in a 2D arry for the game board

        string filename;
        cout << "Please enter a file name: ";	//user is asked for the name of the file containing the sudoku table
        cin >> filename;

        ifstream myfile;
	ifstream newfile(filename.c_str());	//filename is converted to a c-style string

        myfile.open(filename.c_str());		//opens the file for reading
	if(!myfile){
		cout << "Could not open file." << endl;	//checks if the file could be opened or not
		exit(1);
	}
	
	int count = 0;
	while(!myfile.eof()){		//counts the number of elements in the table, either 9x9 or 4x4
		T temp;
		myfile >> temp;
		count++;
	}

	int sizeofboard;
	if (count==82) sizeofboard = 9;
	else sizeofboard = 4;
	boardsize = sizeofboard;

	vector<T> tempVec;

	while(!newfile.eof()){				//elements from file are placed into 2D vector array
		for (int i = 0; i < sizeofboard; i++){

			T tempVar;			//elements are templated, either integers or chars
			newfile >> tempVar;
			tempVec.push_back(tempVar);	//elements are put into a temporary vector and placed into puzzle 

		}
		puzzle.push_back(tempVec);	
		tempVec.clear(); 		
	}

	vector<T> temp1Dvec;			//temporary 1D vector for a row
	vector<vector<T> > temp2Dvec;		//temporary 2D vector for each number 1-9

	for(int i = 0; i < 9; i++){		//initiateds 3D vector of possible values for each cell on the sudoku board 
		for(int j = 0; j < 9; j++){
			for(int k = 0; k < 9; k++){
				T tempVariable = 1;
				temp1Dvec.push_back(tempVariable);		
			}	
			temp2Dvec.push_back(temp1Dvec);
			temp1Dvec.clear();
		}
		possibilities.push_back(temp2Dvec);
		temp2Dvec.clear();
	}


}





template <typename T>
void Puzzle<T> :: display() {				//sudoku table printed on to the moniter

	for(int i = 0; i < getboardsize(); i++){
		for(int j = 0; j < getboardsize(); j++){
			cout << puzzle[i][j] << " ";			//elements are printed with a space between them
			if(getboardsize()==9 && (j==2 || j==5)){	//a vertical bar is placed between collumns 3 and 4 adnd 6 and 7 to separate minigrids
				cout << "| ";
			}
		}
		cout << "\n";
		if(getboardsize()==9 && (i==2 || i==5)){
			cout << "---------------------" << endl;	//horizontal divider is placed between rows 3 and 4 and 6 and 7 to separate minigrids
		}

	}	

}




template <typename T>
int Puzzle<T> :: checkregion(int r, int c,T n){		//checks if a number already exists in a given minigrid
	int m;
	int l;

	//checks the minigrid that the given indices are in, sets new indices to top left element of the minigrid	

	if(r <= 3 && c <=3){	//top left minigrid
		m = 0;
		l = 0;
	}
	else if(r<=3 && c>=4 && c<=6){	//top middle minigrid
		m = 0;
		l = 3;
	}
	else if(r<=3 && c>=7 && c<=9){	//top right minigrid
		m = 0;
		l = 6;
	}
	else if(r<=6 && r>=4 && c<=3){	//middle left minigrid
		m = 3;
		l = 0;
	}
	else if(r<=6 && r>=4 && c<=6 && c>=4){	//center minigrid
		m = 3;
		l = 3;
	}
	else if(r<=6 && r>=4 && c>=7 && c<=9){	//middle right minigrid
		m = 3;
		l = 6;
	}
	else if(r>=7 && c<= 3){	//bottom left minigrid
		m = 6;
		l = 0;
	}
	else if(r>=7 && c>=4 && c<=6){	//bottom middle minigrid
		m = 6;
		l = 3;
	}
	else if(r>=7 && c>=7){	//bottom right
		m = 6;
		l = 6;
	}

	
	for (int i = 0; i < 3; i++){		//checks if an element in the minigrid is equal to the number trying to be placed
		for (int j = 0; j < 3; j++){
			if(puzzle[m+i][l+j]==n) return 1;
		}
	}
	return 0;


}
template <typename T>
int Puzzle<T> :: playable(int r, int c, T n){		//determinds if a number can be place at a given place on the sudoku board
	for(int i = 0; i < getboardsize(); i++){
		if (puzzle[r-1][i]==n){			//if the number already exists in the same row, the number cannot be placed
			//cout << "row" << endl;
			return 0;
		}
		else if (puzzle[i][c-1]==n){		//if the number already exists in the same column, the number cannot be placed
			//cout << "column" << endl;
			return 0;
		}
	}
	if(checkregion(r,c,n)){
		 //cout << "minigrid" << endl;
		 return 0;				//if the number already exists in the same minigrid, the number cannot be placed

	}
	return 1;
}


template <typename T>
int Puzzle<T> :: checkcompletion(){			//checks if the board is completely filled

	for(int i = 0;i < getboardsize(); i++){
		for(int j = 0; j < getboardsize(); j++){
			if(puzzle[i][j]==0) return 0;		//if any element is still equal to zero, then the board is not filled
		}
	}	
	return 1;
}


template <typename T>
void Puzzle<T> :: interactive() { 	//interactive mode for the user to play sudoku

	while(1){
		display();

		//user is asked for the element they want to change based on row and column and what they want to change the element to
		
		cout << "What row do you want to change? (-1 to quit)" << endl;	
		int r; 
		cin >> r;
		if(r==-1) break;
		
		cout << "What column do you want to change? " << endl;
		int c;
		cin >> c;

		cout << "What do element do you want to put in? " << endl;
		T n;
		cin >> n;
		
		if (playable(r,c,n)){		//number is checked if it can be place before it is changed
			puzzle[r-1][c-1] = n;	
		}

		else cout << "This number is not playable here." << endl;	//if number is unable to be placed, an error message is printed

		if (checkcompletion()){						//after each move, the game board is checked if it is completed
			cout << "You completed the sudoku board!" << endl;
			display();
			break;
		}

	}
}

template <typename T>
void Puzzle<T> :: update3Dvector(){		//updates the 3D vector of possiblities if any changes to the board have occured

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			for(int k = 0; k < 9; k++){
				T n = k+1;
				if(playable(i+1,j+1,k+1)){		//if a number is a possible option for a cell, it is designated as a 1
					possibilities[i][j][k] = 1;
				}
				else possibilities[i][j][k] = 0;	//if its not possible it is designated as a 0
			}
		}
	}
}
template <typename T>
void Puzzle<T> :: oneoption(int i, int j){	//for a given cell on the sudoku board, determines the number of possible values and changes the cell if there is only one option
	int counter = 0;
	int number;

	update3Dvector();

	for(int m = 0; m < 9; m++){			//counts the number of possibilites for the given cell
		if(possibilities[i][j][m]==1){		
			number = m+1;			//if a number is a possible value, then its value is stored as a variable
			counter++;
		}
	}


	if (counter==1 && playable(i+1,j+1,number)){	//if there is only one option, the cell is changed to the value that was stored in the variable
		puzzle[i][j] = number;
	
	}
}	


template <typename T>
void Puzzle<T> :: singlepossibility(){		//solves a sudoku board using the single possibility method

	int counter = 0;
	while(!checkcompletion()){		//runs until the board is completed

		for(int i = 0;i < getboardsize();i++){
                	for(int j = 0; j < getboardsize();j++){
				if(puzzle[i][j]!=0) continue;	//skips any number already on the board
				oneoption(i,j);
			}

		}
		counter++;
		if (counter==100){
			cout << "Could not solve puzzle after 100 iterations" << endl;
			 break;
		}
	}
	cout <<"solution took " << counter << " iterations" << endl;	//keeps track of how long it takes to solve the board
	display();
}





template <typename T> 
void Puzzle<T> :: singleton(){			 //singleton method of solving sudoku puzzles		  

	/*checks all the rows, columns, and minigrids and the open cells in them. If a number is only a possible option in
	one of the cells, then that cell is changed to that number.*/


	for(int i = 0; i < 9; i++){		//singleton method for just the rows
		int temparray[10] = {0};	//temporary array keeps track of how many times each number 1-9 is possible in each for a given row

		for(int j = 0;j < 9; j++){
			if(puzzle[i][j]!=0) continue;	//if a cell already has a number it is ignored	
			else{
				for(int k = 0; k < 9; k++){
					if(possibilities[i][j][k]==1) temparray[k]++;	//for a given cell counts how many times each number is possible
				}
			}
		}
		for(int m = 0; m < 9; m++){
			if(temparray[m]==1){					//if a number is only possible once in a row, that number is added to the sudoku board
				for(int a = 0; a < 9; a++){
					if(playable(i+1,a+1,m+1) && puzzle[i][a]==0){	//finds the cell in the row that the number is playable
						puzzle[i][a] = m+1;
					}
				}
			}
		}
				
	}
	update3Dvector();	//possiblities 3D vector is updated after singleton for rows

	for(int x = 0; x < 9; x++){			//singleton method for just the columns
                int temparray[10] = {0};
                for(int y = 0;y < 9; y++){
                        if(puzzle[y][x]!=0) continue;
                        else{
                                for(int z = 0; z < 9; z++){
                                        if(possibilities[y][x][z]==1) temparray[z]++;	//counts the number of times each number is a possibility in the column
                                }
                        }
                }
                for(int n = 0; n < 9; n++){
                        if(temparray[n]==1){						//if a number is only possible once in a column it is added on the sudoku board
                                for(int b = 0; b < 9; b++){
                                        if(playable(b+1,x+1,n+1) && puzzle[b][x]==0){	//finds the cell in the column where number is playable, then makes the change
						puzzle[b][x] = n+1;
        				}
	                        }
                        }
                }

        }

	update3Dvector();	//possibilities vector is updated

	for(int q = 0; q < 7; q+=3){			//singleton method for just the minigrids
		for(int r = 0; r < 7; r+=3){
			int temparray[10] = {0};
			for(int g = 0 ; g < 3; g++){		//when checking a minigrid, it always starts at the top left cell of the minigrid
				for(int h = 0; h < 3; h++){
					if(puzzle[q+g][r+h]!=0) continue;
					else {
						for(int f = 0; f < 9; f++){
							if(possibilities[q+g][r+h][f]==1) temparray[f]++;	//counts number of times each number is possible in the minigrid
						}
					}
				}
			}
			for(int p = 0; p < 9; p++){
				if(temparray[p]==1){				//if a number is only possible once in minigrid, that number is added to the sudoku board
					for(int u = 0; u < 3; u++){
						for(int v = 0; v < 3; v++){
							if(playable(q+u+1,r+v+1,p+1) && puzzle[q+u][r+v]==0){	//determines where on the minigrid that number is playable, then makes the change
								puzzle[q+u][r+v]=p+1;
							}
						}
					}
				}
			}
		}
	}
	update3Dvector();	//possibilities vector is updated

}


template <typename T> 
void Puzzle<T> :: solver(){		//solves medium and hard level sudoku boards using the singleton method along with the single elimination method
	
	int counter = 0;	
	while(!checkcompletion()){			//the single elimination method and singleton method alternated until the board is fille up
		 for(int i = 0;i < getboardsize();i++){
                        for(int j = 0; j < getboardsize();j++){
                                if(puzzle[i][j]!=0) continue;  		//don't change any numbers alreay on the board
                                oneoption(i,j);			//single elimination method
                        }

                }

		singleton();				//singleton method

		counter++;
		if(counter==100){			//if puzzle can't be solved after 100 iterations,it most likely cannot be solved with either of these methods
			cout << "Could not solve puzzle after 100 iterations." << endl;
			break;
		}	
	}
	cout << "This puzzle took " << counter << " iterations" <<  endl;
	display();


}

#endif
