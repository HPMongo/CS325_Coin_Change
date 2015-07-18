/*
CLASS: CS 325
PROGRAM: PROJECT 2
DUE DATE: 07/21/2015
GROUP MEMBERS: HUY PHAM, MICHAEL SHARKAZY, BRYAN SERIF

This program implents three ways of solving the coin changing problem.
First is by brute force which is changeSlow
Second is by a greedy algorithm which is changeGreedy
Third is by dynamic programming which is changeDP

Note: Algorithms may produce differing results depending on the problem
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>

#define MAXSIZE 20

void outputCoinArray(int cca[], int length, int count, char arg[]);
int changeSlow();
int changeGreedy(int v[], int cca[], int &amount, int &length);
int changeDP();


int main(int argc, char *argv[])
{
	int values[MAXSIZE];
	int coinCountArray[MAXSIZE];
	int amount, length, coinCount, intHolder;
	std::ifstream input;
	std::string s, filename;

	if (argc < 2)
	{
		std::cout << "\nNo filename is present! Filename must be included in command\n"
			<< "line when executing program!\n\n"
			<< "Program Terminated!\n\n";

		return 1;
	}



	input.open(argv[1]); // input filename = argv[]1 from command line

	if (!input.fail())
	{

		while (std::getline(input, s))	// get the first line of the input file
		{
			std::stringstream ss;

			/*This is dependant on a properly formatted input file in order
			to run correctly. The format must be:
			[int, int, int]
			int
			*/

			// here we find and remove the brackets the ready the input string for atoi
			int found = s.find('[');
			s.erase(found, 1);
			found = s.find(']');
			s.erase(found);

			ss.str(s);	// copy the string into the stringstream

			/*Set needed length of the array to 0. This will increment
			every time a new integer is input into the array.*/
			length = 0;

			/*This while loop using the delimeter of ',' and extracts each string "int"
			from the stringstream back into the string. It then uses atoi() to convert
			the string "int" into and int. That int is the place into the values array
			at length. Length is then incremented*/
			while (std::getline(ss, s, ','))
			{
				intHolder = atoi(s.c_str());

				values[length] = intHolder;

				length++;
			}
			/* After the above line is taken into the array we then take the next line in
			the input file which is the amount we are searching for and put it into amount.*/
			std::getline(input, s);
			amount = atoi(s.c_str());

			//call the algorithm and then call the outputting function
			coinCount = changeGreedy(values, coinCountArray, amount, length);
			outputCoinArray(coinCountArray, length, coinCount, argv[1]);

			/*THE OTHER CALLS MAY NEED TO BE MODIFIED BEFORE HAND OR IN THE ALGORITHM
			TO ACCOUNT FOR ONLY USING THE SAME COIN COUNT ARRAY FOR ALL OF THEM. E.G. IF
			YOU MODIFY ONE VALUE ON THE FIRST TEST, BUT THEN IT DOES NOT GET MODIFIED ON
			THE SECOND STRING, THE VALUE IN THE ARRAY MAY STILL REFLECT THE PREVIOUS TEST VALUE

			THIS BLOCK COMMENT CAN BE DELETED AFTER PROGRAM COMPLETED.*/
		}

		input.close();
	}

	else
		std::cout << "\nUnable to open file!!!\n\n";
	

	return 0;
}

/*
Output Coin Array

Parameters:
	cca - the coint count array passed through for reading and outputting
	length - the length set by the current coin array size from the input file intake
	count - the total sum or count of coins needed for the solution
	arg[] - the command line argument containing the filename used for input

Process:
	This function takes in the above parameters. It then changes the input file name
	and appends "change.txt" to the end giving you [input filename]change.txt. It opens
	the output under the new filename and writes the coin count array and total count
	of coins to the output file and the console;

*/

void outputCoinArray(int cca[], int length, int count, char arg[])
{
	std::ofstream output;
	std::string filename;

	filename = arg;
	filename.erase(filename.end() - 4, filename.end());
	filename += "change.txt";

	output.open(filename.c_str(), std::ofstream::app);

	std::cout << "[";
	output << "[";

	for (int i = 0; i < length; i++)
	{
		std::cout << cca[i];
		output << cca[i];
		if (i < length - 1) { 
			std::cout << ", ";
			output << ", ";
		}
	}
	std::cout << "]\n" << count << "\n\n";
	output << "]\n" << count << "\n\n";

	output.close();
}

/*
Change Greedy

Parameters:
	v - the array of values as read in from the input file [low -> high]
	cca - the coin count array
	amount - the amount solution to be searched for as read in from the input file
	length - the necessary length for the value and coin count arrays

Process:
	This function takes in the above parameters. It then searches from the highest
	value in the value array to the lowest determining if the highest value can be
	used in the amount. It then subtracts the maximum that coin amount can be used
	and repeats with the next lowest coin value until amount = 0. It then returns
	the aggregate amount of coins needed to make change.
*/

int changeGreedy(int v[], int cca[], int &amount, int &length)
{
	int count, aggregateCount = 0;

	for (int i = length - 1; i >= 0; i--)
	{
		if (v[i] <= amount)
		{
			count = (amount / v[i]);
			aggregateCount += count;
			cca[i] = count;
			amount -= (count * v[i]);
		}

		else { cca[i] = 0; }
	}	

	return aggregateCount;
}