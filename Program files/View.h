#pragma once
#include <string>
#include <iomanip>
#include <iostream>
#include <vector>
#include <set>
#include "stdafx.h"

using namespace std;

class View
{
public:
	//pre: none
	//post: program gets user selection (error checked)
	//returns: 1 for analysis and 2 for suggested move
	int getOption(); //display prompt and get 1 for move suggest and 2 for analyze board

	//pre: max stone count passed
	//post: user input of board and dice given (values checked). 
	//      user assumed to use proper format (i.e. numbers where numbers go, etc.)
	//returns: 8 character string, first six are board, last two are dice
	string getDieAndBoard(int maxStones); 

	//pre: max stone count passed
	//post: user input of board (values checked). 
	//      user assumed to use proper format (i.e. numbers where numbers go, etc.)
	//returns: 6 character string to represent stone count at each position
	string getBoard(int maxStones);

	//pre: vector<double> containing {min, max, expected value}
	//		and string containing a board are passed
	//post: the max, expected value, and board are displayed for the user
	//returns: none 
	void showSuggestion(vector<double> data, string suggestion);

	//pre: the board and data for each roll is passed
	//post: the board is displayed
	//      data for each roll is display in cloumbs for roll, min, max, and expected value
	//returns:
	void showAnalysis(string board, vector<vector<double>>);

	//pre: none
	//post: the user is asked if they wish to continue using the program or quit
	//returns: true if they want to quit, false otherwise
	bool end(); //get user's decision to do more or exit
};

