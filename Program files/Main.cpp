// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h";

//#include "BoardGenerator.h"
#include "searchControl.h"
#include "View.h"
#include <iomanip>

//#include <list>



int main()
{
	const int  MAXSTONES = 6; //THIS IS WHERE TO CHANGE MAX STONES ACROSS WHOLE PROGRAM
	vector<vector<int>> allBoards = getAllBoards(MAXSTONES); //all possible boards of 0-MAX stones
	View IO;
	searchControl sh;
	bool end = false; //controls program user interface loop

	for each (vector<int> testVec in allBoards)
	{
		sh.getBoardValues(testVec); //populates the search database 
	}

	while (!end)
	{
		int option = IO.getOption(); 
		if (option == 1)  //Suggest move
		{
			string key = IO.getDieAndBoard(MAXSTONES);
			
			string val = sh.database.find(key)->second; //gets database value associated with key
			//val is in the form "min max ev suggested move: 
			int index = val.find(": "); //index of :
			index += 2; //move it to start of suggested board

			string nextBoard = val.substr(index);
			vector<double> data = sh.getDatabaseValue(nextBoard);
			IO.showSuggestion(data, nextBoard);
		}
		else if (option == 2) //Analyze board 
		{
			string board = IO.getBoard(MAXSTONES);
			vector<vector<double>> allRollVals; //will hold min/max/EV info for each roll
			for each (vector<int> roll in sh.genAllRolls())
			{
				string key = board; //first 6 characters of database key
				key += to_string(roll[0]); //7th character of database key
				key += to_string(roll[1]); //8th character of database key
				vector<double> entry = sh.getDatabaseValue(key); 
				entry.push_back(roll[0]); //adds die 1 to the obtained vector
				entry.push_back(roll[1]); //adds die 2 to the optained vector 

				allRollVals.push_back(entry);
			}

			IO.showAnalysis(board , allRollVals);

		}

		end = IO.end();
	}

	return 0; 
}



