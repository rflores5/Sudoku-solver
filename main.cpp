#include <iostream>
#include <vector>
#include <string>
#include "puzzle.h"

int main(){


	Puzzle<int> sud;
	sud.display();
	cout << "Do you want to play or the solution of the sudoku puzzle? (play or solve)" << endl;
	string input;
	cin >> input;
	if (input=="play") sud.interactive();
	else if (input=="solve") sud.solver();
	else cout << "Invalid input";
}
