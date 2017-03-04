#include <iostream>
#include <sstream>
#include <time.h>
#include <limits.h>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

int get_line_breaks(vector<int>& Prev, int wordIndex, vector< vector<int> >& prettyLineIndices) {
	int k, i = Prev[wordIndex];
	if (i == 1)
		k = 1;
	else {
		k = get_line_breaks(Prev, i - 1, prettyLineIndices) + 1;
	}

	prettyLineIndices[k][0] = i;
	prettyLineIndices[k][1] = wordIndex;
	//cout << "k: " << k << "i: " << i << "wordIndex: " << wordIndex << endl;
	return k;
}

//The program assumes the input args as: name of the text file, maximum line length(M).
int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "Incorrect input args list" << endl;
	}

	//Open the input text file.
	ifstream file;
	file.open(argv[1]);
	if (!file.is_open()) return -1;

	//Create an array of the lengths of the input words.
	vector<int> wordLengths;
	wordLengths.push_back(-1);
	string word;
	while (file >> word)
	{
		wordLengths.push_back(word.length());
	}
	file.close();

	//Read the input argument of maximum line length
	size_t numWords = wordLengths.size() - 1;
	istringstream maxLineLen(argv[2]);
	int maxLineLength;
	if (!(maxLineLen >> maxLineLength))
		cerr << "Invalid number for maxLineLength" << argv[2] << '\n';

	//Computing the extra spaces for all combination of words i and j.
	vector< vector<int> > extraSpace(numWords+1, vector<int>(numWords + 1, 0));
	for (size_t i = 1; i <= numWords; i++) {
		extraSpace[i][i] = maxLineLength - wordLengths[i];
		for (size_t j = i + 1; j <= numWords; j++) {
			extraSpace[i][j] = extraSpace[i][j - 1] - wordLengths[j] - 1;
		}
	}

	//Compute the penalty for all combination of words based upon the extra spaces.
	vector< vector<int> > penalty(numWords + 1, vector<int>(numWords + 1, 0));
	for (size_t i = 1; i <= numWords; i++) {
		for (size_t j = i; j <= numWords; j++) {
			if (extraSpace[i][j] < 0) {
				//a large number to singify the minimum of penalty not to 
				//count the case in which words dont fit in the line.
				penalty[i][j] = INT_MAX; 
			}
			else if (j == numWords && extraSpace[i][j] >= 0) {
				//Last line penalty is zero
				penalty[i][j] = 0; 
			}
			else {
				penalty[i][j] = (int)pow(extraSpace[i][j], 3);
			}
		}
	}

	//Calculate the minimum cost of the penalty for each word with storing the line breaks in the array Prev.
	vector<unsigned int> MinCost(numWords + 1);
	vector<int> Prev(numWords + 1);
	MinCost[0] = 0;
	for (size_t j = 1; j <= numWords; j++) {
		MinCost[j] = UINT_MAX;
		for (int i = 1; i <= (int)j; i++) {
			if (MinCost[i-1] + penalty[i][j] < MinCost[j]) {
				MinCost[j] = MinCost[i - 1] + penalty[i][j];
				Prev[j] = i;
			}
		}
	}

	vector< vector<int> > prettyLineIndices(numWords +1, vector<int>(2, 0));
	int num_pretty_lines = get_line_breaks(Prev, numWords, prettyLineIndices);

	//Read the input file to output the words in console based on the algorithm.
	file.open(argv[1]);
	if (!file.is_open()) return -1;

	int i = 1;
	while(i <= num_pretty_lines){
		int startIndex = prettyLineIndices[i][0];
		int endIndex = prettyLineIndices[i][1];
		string wordsLine;
		while (startIndex <= endIndex) {
			string word;
			if(file >> word)
			{
				wordsLine.append(word);
				wordsLine.append(" ");
			}
			startIndex++;
		}
		cout << wordsLine << endl;
		i++;
	}
	file.close();

	i = 1;
	int finalPenalty = 0;
	while (i < num_pretty_lines) {
		int startIndex = prettyLineIndices[i][0];
		int endIndex = prettyLineIndices[i][1];
		
		finalPenalty += penalty[startIndex][endIndex];
		i++;
	}

	cout << "penalty: "<< finalPenalty << endl;
	return 0;
}