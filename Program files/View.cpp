#include "stdafx.h"
#include "View.h"
#pragma once

int View ::getOption()
{
	int  choice;
	bool valid;
	do
	{
		valid = true;
		cout << "Please select one of the following options: \n 1 Suggest Move \n 2 Analyze Board \n\n Selection: ";
		cin >> choice;
		if (choice != 1 &&  choice != 2)
		{
			valid = false;
			cout << endl << "Invalid input" << endl; 
		}

	} while (!valid);

	return choice;
}

void View::showAnalysis(string board, vector<vector<double>> data)
{
	string dice;
	cout << endl << "For the board " << board << " the following values are turns remaining not including the current turn : \n\n";
	cout << "Dice Values | Minimum Possible Turns | Maximum Possible Turns | Expected Number of Turns" << endl;

	for (int i = 0; i < data.size(); ++i)
	{
		cout << "----------------------------------------------------------------------------------------" << endl; 
		cout << "     " << data[i][3] << data[i][4] << setw(21) << data[i][0] << setw(21) << data[i][1] << setw(23) << data[i][2] << endl;
		cout << endl;
	}
	
}

void View::showSuggestion(vector<double> data, string suggestion)
{
	string max = to_string(data[1]);
	max = max.substr(0, max.find('.')); //trim excess
	string ev = to_string(data[2]);
	ev = ev.substr(0, (ev.find('.') + 4)); //trim values after 3rd decimal place 
	
	cout << endl << "Suggested move is " << suggestion << " with max turns remaining of "
		<< max << " and expected remaining turns of " << ev << endl; 
}

string View::getBoard(int maxStones)
{
	string board;
	string max = to_string(maxStones);
	int numStones;
	bool valid;
	do
	{
		numStones = 0;
		valid = true;
		cout << endl << "Please enter your board (six numbers, no spaces) : ";
		cin >> board;

		for (int i = 0; i < 6; ++i)
		{
			numStones += int(board[i]) - 48;
		}

		if (numStones > maxStones)
		{
			valid = false;
			cout << endl << "Error: numbers entered must total to " << max << " or less. Try again.";
		}

		if (board.length() > 6)
		{
			valid = false;
			cout << endl << "Error: Boards can only display six positions";
		}


	} while (!valid);


	return board ;

}

string  View::getDieAndBoard(int maxStones)
{
	string board = getBoard(maxStones);
	string dice;

	bool valid;
	do
	{
		valid = true;
		
		cout << endl << "Please enter the die roll (two numbers, no spaces) : ";
		cin >> dice;

		int dieOne = int(dice[0]) - 48;
		int dieTwo = int(dice[1]) - 48;

		if (dieOne > 6 || dieTwo > 6 || dieOne < 1 || dieTwo < 1)
		{
			valid = false;
			cout << endl <<  "Error: Die values must be between 1 and 6. Try again";
		}

		//make them follow the convention of min first then max
		if (dieOne > dieTwo)
		{
			char temp = dice[1];
			dice[1] = dice[0];
			dice[0] = temp;
		}
		

	} while (!valid);

	
	return board + dice;
	
}

bool View::end()
{
	int choice;
	bool valid;
	do
	{
		valid = true;
		cout << endl << "Would you like to continue using the program? \n 1 for yes \n 2 for no" << endl
			<< "Your choice: ";
		cin >> choice;

		if (choice != 1 && choice != 2)
		{
			valid = false;
			cout << endl << "Error: Invalid choice. Try again.";
		}
	} while (!valid);

	if (choice == 1) return false;

	return true;
	
}