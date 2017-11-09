#include "BoardGenerator.h"

#pragma once

using namespace std;

class searchControl {
private: 
	
	const vector<int> ALLZERO = { 0,0,0,0,0,0 };

public:


	// the database holds pairs with board info and min/max/expectedValue/suggestion information
	unordered_map< std::string, std::string> database; 

	//pre: none
	//post: all dice values are generated
	//returns: 2D vector with rolls (doubling of doubles considered) { {1,1,1,1} , {1,2}, ... {6,6}}
	vector<vector<int>>  genAllRolls()
	{
		int total = 0;
		vector<vector<int>> result;
		for (int i = 1; i < 7; ++i)
		{
			for (int j = i; j < 7; ++j)
			{
				vector<int> temp;
				temp.push_back(i);
				temp.push_back(j);
				if (i == j) { temp.push_back(i); temp.push_back(i); }
				result.push_back(temp);
				++total;
			}
		}
		return result;
	}

	
	searchControl() { database.insert(make_pair<string, string>("000000", "0 0 0 ")); } //seed all values 0 at complete

	//pre: user passes a vector 
	//post: the vector is iterated through to make a string 
	//returns: the string with each vector element 
	static string vecToString(vector<int> v)
	{
		string s = "";
		for (int i = 0; i < v.size(); ++i) s += to_string(v[i]);
		return s;
	}
	
	//pre: key to the database of 6 characters for a board or 8 for a board and roll is passed
	//     It is assumed that the string is valid and therefore has a corresponding key 
	//post: the value associated with the key is obtained and parsed for the min, max, and expected value 
	//returns: vector<double> of {min, max, expectedValue}
	vector<double> getDatabaseValue(string s)
	{
		vector<double> returnVal;

		//The database will return a string in the form of "min max expectedvalue suggestion" with the last as optional
		string res = database[s];
		int spaces = 0;
		int nav = 0; //navigates the obtained string character by character
		string num = "";

		do //get min and max values (the first 2) 
		{
			if (res[nav] == ' ') //if it is a space, we've reached the end of min or max
			{
				++spaces;
				returnVal.push_back(stoi(num));
				num = "";
			}
			else num += res[nav];

			++nav;
		} while (spaces < 2);

		int suggestedPos = res.find(" suggested"); //see if there's a suggested move 
		//if suggested is there, get values from after max and before suggested (the expected value)
		if (suggestedPos) returnVal.push_back(stod(res.substr(nav, (suggestedPos - nav) ))); 
		else returnVal.push_back(stod(res.substr(nav))); //if no suggestion, the rest is part of expected Value

		return returnVal;
	}

	//pre: data for a new entry in the database, including min/max/expectedValue is passed
	//	   relevant dice values and a suggested move is also passed if it is a board and die entry 
	//post: information is compiled into a key and value pair and inserted into the database
	//returns: none 
	void addDatabaseValue(vector<double> vals, vector<int> board, vector<int> dice = { 0,0 } , string next = "")
	{

		pair<string, string> keyval("", "");
		for (int i = 0; i < board.size(); ++i)
		{
			keyval.first += to_string(board[i]); //board values make up the first part of all keys
		}

		//if dice are given, they are added to the key. Otherwise, they are 0 by default and ignored
		if (dice[0] != 0) keyval.first += to_string(dice[0]) + to_string(dice[1]);

		//values are passed by vector as {min, max, expectedVal}
		string minString = to_string(vals[0]);
		string maxString = to_string(vals[1]);
		string evalString = to_string(vals[2]);

		minString = minString.substr(0, minString.find('.')); //trim the decimal point
		maxString = maxString.substr(0, maxString.find('.')); //trim the decimal point
		evalString = evalString.substr(0, evalString.find('.') + 4); //expected value trimmed to 3 places 
		keyval.second += minString + " " + maxString + " " + evalString; // values compiled into a string
		if (next != "") keyval.second += " suggested move: " + next; //suggestion added when applicable 
		database.insert(keyval); 

	}

	//pre: a valid board and vector of rolls is passed
	//post: board and roll is assessed to find the lowest expected turns option and the min and max turns
	//      The database is given the board, roll, and suggestion information 
	//returns: vector<double> of format {min, max, expected value}
	vector<double> findRollValues(vector<int> board, vector<int> roll)
	{
		//generate all possible boards using given board and roll
		set<vector<int>> allNext = getBoards(board, roll);
		double min = INFINITY;
		double max = -1;
		double EV = INFINITY;
		string bestNext; //holds the board with lowest EV examined
		
		for each (vector<int> next in allNext) //for each possible board using these dice and board
		{
			vector<double> vals = getBoardValues(next); 
			if (EV > vals[2]) //if this option's expected value is lowest, choose it 
			{
				min = vals[0];
				max = vals[1];
				EV = vals[2];
				bestNext = vecToString(next);
			}
		}

		vector<double> toAdd = { min, max, EV };
		addDatabaseValue(toAdd, board, roll, bestNext); //record into database 
		return toAdd; //return values 
	}

	//pre: valid board is passed
	//post: database is checked and it's value is returned if it already exists
	//		otherwise, each possible roll is examined and the min, max, and expected values are produced
	//returns: vector<double> with values {min, max, expectedValue}
	vector<double> getBoardValues(vector<int> board)
	{
		string boardString = vecToString(board);
		//check the database in case we've been in this board before
		if (database.find(boardString) != database.end())
		{
			return getDatabaseValue(boardString);
		}

		vector<vector<int>> allRolls = genAllRolls(); 
		double EV = 0; 
		double min = INFINITY;
		double max = -10;

		for each (vector<int> dice in allRolls) //for every possible dice combination
		{
			vector<double> temp = findRollValues(board, dice);
			if (temp[0]+1 < min) min = temp[0]+1; //if this min is lower, replace prior min
			if (temp[1]+1 > max) max = temp[1]+1; //if this max is higher, replace prior max

			if (dice[0] != dice[1]) EV += temp[2]+1; //non-doubles are 2x weight
			EV += temp[2]+1;  //totalling expected value to divide later
		}

		EV /= 36; //divide total weighted value with the die combinations 
		vector<double> toAdd = { min, max, EV }; //compile values into vector 
		addDatabaseValue(toAdd, board); //record into database 

		return toAdd;
	}

	
	
};