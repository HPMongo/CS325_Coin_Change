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
#include <limits>
#define MAXSIZE 20

void outputCoinArray(int cca[], int length, int count, char arg[]);
int changeSlow(int v[], int cca[], int amount, int length);
int changeGreedy(int v[], int cca[], int &amount, int &length);
int changeDP(int v[], int cca[], int amount, int length);
void initArray(int arr[], int length);

int main(int argc, char *argv[])
{
	int values[MAXSIZE];
	int coinCountArray1[MAXSIZE];
	int coinCountArray2[MAXSIZE];
	int coinCountArray3[MAXSIZE];
	initArray(coinCountArray1, MAXSIZE);
	initArray(coinCountArray2, MAXSIZE);
	initArray(coinCountArray3, MAXSIZE);
	
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
			int amt;

			//call the slow (divide and conquer) algorithm then call the output
			//function
			amt = amount;
			coinCount = changeSlow(values, coinCountArray2, amt, length);
			outputCoinArray(coinCountArray2, length, coinCount, argv[1]);

			//call the greedy algorithm and then call the output function
			amt = amount; 
			coinCount = changeGreedy(values, coinCountArray1, amt, length);
			outputCoinArray(coinCountArray1, length, coinCount, argv[1]);

			//call the dynamic programming algorithm then call the output 
			//function
			amt = amount;
			coinCount = changeDP(values, coinCountArray3, amt, length);
			outputCoinArray(coinCountArray3, length, coinCount, argv[1]);
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
void initArray(int arr[], int length)
{
	for (int i = 0; i < length; i++)
	{
		arr[i] = 0;
	}
}
/*
Change Slow

Parameters:
	v - the array of values as read in from the input file [low -> high]
	cca - the coin count array
	amount - the amount solution to be searched for as read in from the input file
	length - the necessary length for the value and coin count arrays

Process:
	This function takes in the above parameters. It then searches the entire array of 
	values and determine the best (minimum number of coins) using brute-force (divide
	and conquer) appoarch.
*/

int changeSlow(int v[], int cca[], int amount, int length)
{
	// Assign default value to total coin count
	int aggregateCount = std::numeric_limits<int>::max();
	
	// Base case - search the array of values. If the coin denomination matchs with the search
	// value, then return the current coin as it is the lowest possible result.
	// Update the coin count array with the coin number
	for (int i = 0; i < length; i++)
	{
		if (amount == v[i])
		{
			cca[i] += 1;
			return 1;
		}
	}
	
	// Recursive case - find all values as long as the value is less than search
	// amount. Find the minimum of coins to make i cents, then find the minimum
	// of coins to make amount - i cents. Compare the result against the best
	// solution. If the result is better than the current best solution, replace
	// best solution and update the coin change array. 
	for (int i = 1; i < amount; i++)
	{
		int ccaI[length];
		int ccaKI[length];
		initArray(ccaI, length);
		initArray(ccaKI, length);
		int icent = changeSlow(v, ccaI, i, length);
		int k_icent = changeSlow(v, ccaKI, amount - i, length);
		if(icent + k_icent < aggregateCount) 
		{
			aggregateCount =  icent + k_icent;
			for(int j = 0; j < length; j++)
			{
				ccaI[j] += ccaKI[j];
			}
			for(int k = 0; k < length; k++)
			{
				cca[k] = ccaI[k];
			}
		}
	}	
	return aggregateCount;
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

/*
Change Dynamic Programming

Parameters:
	v - the array of values as read in from the input file [low -> high]
	cca - the coin count array
	amount - the amount solution to be searched for as read in from the input file
	length - the necessary length for the value and coin count arrays

Process:
	This function takes in the above parameters. It then calculates the number of coins 
	needed for each value from 0 to n-amount using dynamic programming - bottom up method.
	It also keeps track of the coin that is used for each value. Once the function has 
	the minimum amount of coins for a given amount, it will back track the used coins array
	to figure out the actual coins that were used.
*/

int changeDP(int v[], int cca[], int amount, int length)
{
	// Create arrays to store minimum amount and coins used
	int minCoin[amount + 1];
	int coinUsed[amount + 1];
	// Initialize the min coin array [0] with 0 as this is the starting position
	minCoin[0] = 0;
	// Initialize the rest of the min coin array with high value
	for (int i = 1; i < amount + 1; i++)
	{
		minCoin[i] =  std::numeric_limits<int>::max();
	}	
	// Initialize coin used array and coin change array
	initArray(coinUsed,amount + 1);
	initArray(cca,length);

	// Calculate the minimum amount of coin needed for each amount 0 to n (given amount)
	for (int i = 1; i <= amount; i++)
	{
		for (int j = 0; j < length; j++)
		{
			if (v[j] <= i)
			{
				if (minCoin[i - v[j]] + 1 < minCoin[i])
				{
					minCoin[i] = minCoin[i - v[j]] + 1;
					coinUsed[i] = j;
				}
			}
		}
	}
	
	// Back track the actual coins that were used in the calculation
	int n = amount;
	int temp = 0;	
	while (n > 0)
	{
		temp = v[coinUsed[n]];
		for (int i = 0; i < length; i++)
		{
			if (temp == v[i])
			{
				cca[i] += 1;
			}
		}
		n = n - v[coinUsed[n]];
	}
	return minCoin[amount];
}	
