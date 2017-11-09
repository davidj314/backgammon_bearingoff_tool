#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>
#include <string>

#pragma once
using namespace std;

//unordered_map< std::string, std::string> database;
//keys are strings of 6 or 8 characters, depending on if dice are included 


//Pre: a valid board is passes along with a position and die value(hops)
//Post: stone is moved from passed position as many places as hops
//returns: the board that results from the desired move
vector<int> moveStone(vector<int> places, int position, int hops)
{
	vector<int> temp = places;
	--temp[position];
	if (position - hops > -1) ++temp[position - hops];
	return temp;
}

//pre: a valid board is passed
//post: a stone at the highest occupied position is removed from play
//returns: a vector<int> representing the resulting board
vector<int> popStone(vector<int> stones)
{
	vector<int> result = stones;
	for (int i = stones.size() - 1; i > -1; --i) //search positions starting at end 
	{
		if (stones[i] > 0) //found the first occupied position
		{
			--result[i]; //remove stone
			i = -1; //end the search
		}
	}
	return result; 
}

vector<vector<int>> getAllBoards(int maxStones)
{
	const static int MAXSTONES = maxStones;
	vector<vector<int>> result;
	int totalBoards = 1;
	vector<int> positions = { 0,0,0,0,0,0 };
	do {
		int sum = 0; //for counting the total stones of a board
					 //if last is value of MAXSTONES, convert all  at end to 0 and ++ the first non-max
		if (positions[5] == MAXSTONES)
		{
			for (int j = 5; j > -1; --j) //go through each position until we find one not maxed 
			{
				if (positions[j] == MAXSTONES) //if max, set to 0 and continue
				{
					positions[j] = 0;
				}
				else  //we've found highest position not at max. increment and stop
				{
					++positions[j];
					j = -1;
				}
			}
		}

		else { ++positions[5]; } //if last position isn't at max, simply increment it 


		for (int counter = 0; counter < 6; ++counter) //if they 
		{
			sum += positions[counter];
		}

		if (sum <= MAXSTONES) // if sum is <= MAXSTONES, it is a board that will eventually be in database
		{
			result.push_back(positions);
			totalBoards++;
		}

	} while (positions[0] < 6);

	return result;
}

//pre: a valid board and die value is passed
//post: a set of boards resulting from the full use of the die is made
//returns: the set of all boards resulting from fully using the die 
set<vector<int>> genAll(vector<int> places, int hops)
{
	set<vector<int>> result;
	bool hasStone = false;
	for (int i = hops-1; i < places.size(); ++i) //search board of all places allowing full utilization 
	{
		if (places[i] > 0)
		{
			result.insert(moveStone(places, i, hops));
			hasStone = true;
		}
	}
	return result;
}

//pre: valid board is passed as B and valid die combo is passed (vector of 4 if doubles, 2 if not)
//	   the first die is lower or same as the second 
//post: all possible next positions using that board and dice are generated
//returns: set of vector<int> is returned, each vector<int> being a resulting board
set<vector<int>> getBoards(vector<int> B, vector<int> dice)
{
	set<vector<int>> intermediateBoards; //boards made during the process but not done
	set<vector<int>> resultingBoards; //boards using all dice and are valid next states

	const vector<int> ALLZERO = { 0,0,0,0,0,0 };

	//the second die is always the max 
	int highDie = dice[1];
	int lowDie = dice[0];

	int highDieStones = 0;
	int lowDieStones = 0;

	//find out how many stones can use the high and low dice 
	for (int i = highDie - 1; i < B.size(); ++i)highDieStones += B[i];

	for (int i = lowDie - 1; i < B.size(); ++i) 
	{ 
		lowDieStones += B[i];
		if (B[i] > 0 && highDie + lowDie <= i) ++lowDieStones; //if dice add up to <= i, both can be used fully on same stone
	}
	
	if (highDieStones > 0) //entered if high die can be fully used
	{
		intermediateBoards = genAll(B, highDie); //get all boards using full high die
		for each (vector<int> vec in intermediateBoards)
		{
			if (lowDieStones > 1) //if low die can be fully used
			{
				set<vector<int>> temp = genAll(vec, lowDie);
				for each (vector<int> finalVec in temp)
				{
					if (finalVec == ALLZERO)  //if we can reach 000000 we're done and only need that one
					{ 
						resultingBoards.clear(); 
						resultingBoards.insert(ALLZERO); 
						return resultingBoards; 
					}
					resultingBoards.insert(finalVec);
				}
			}
			else //low die cant be used fully
			{
				vector<int> m = popStone(vec);
				if (m == ALLZERO)  //if we can reach 000000 we're done and only need that one
				{ 
					resultingBoards.clear(); 
					resultingBoards.insert(ALLZERO); 
					return resultingBoards; 
				}
				resultingBoards.insert(m);
			}
		}
	}
	else if (lowDieStones > 0 ) //entered if low die can be fully used and not high
	{
		intermediateBoards = genAll(B, lowDie); //gets all boards using full low die
		for each (vector<int> vec in intermediateBoards)
		{
			vector<int> temp = popStone(vec); //high die takes stone from biggest position remaining
			if (temp == ALLZERO) //if we can get to completion, it is all that matters 
			{ 
				resultingBoards.clear(); 
				resultingBoards.insert(ALLZERO); 
				return resultingBoards; 
			}
			resultingBoards.insert(temp);
		}
	}

	else  //entered if both dice cant be utilized fully 
	{
		B = popStone(B);
		B = popStone(B);
		if (B == ALLZERO)
		{
			resultingBoards.clear();
			resultingBoards.insert(ALLZERO);
			return resultingBoards;
		} 

		resultingBoards.insert(B);
	}

	//now we've used two dice.
	//if the dice vector is size 4, that means we need to call this method on all currently collected boards

	if (dice.size()>2) //if true, we are in first pass of handling doubles 
	{
		set<vector<int>> done; //will hold new boards 
		vector<int> v = { dice[0], dice[1] };
		if (resultingBoards.size() == 0) resultingBoards.insert(B);
		for each (vector<int> vect in resultingBoards) //for each of these results
		{
			set<vector<int>> temp = getBoards(vect, v);  //do all this again
			for each (vector<int> nextBoard in temp) //for each in result
			{
				if (nextBoard == ALLZERO) //if we can get to complete, it's all that matters
				{
					done.clear();
					done.insert(nextBoard);
					return done;
				}
				done.insert(nextBoard);
			}
		}
		resultingBoards = done; //replace with the final boards 
	}
	return resultingBoards;
}