#include <iostream>
#include <sstream>
#include <time.h>
#include <limits.h>
#include <fstream>
#include <cstdlib>
//#include <chrono>
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
	cout << "k: " << k << "i: " << i << "wordIndex: " << wordIndex << endl;
	return k;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		cout << "Incorrect input args list" << endl;
	}

	ifstream file;
	file.open(argv[1]);
	if (!file.is_open()) return -1;

	vector<int> wordLengths;
	wordLengths.push_back(-1);
	string word;
	while (file >> word)
	{
		wordLengths.push_back(word.length());
	}

	size_t numWords = wordLengths.size() - 1;
	istringstream maxLineLen(argv[2]);
	int maxLineLength;
	if (!(maxLineLen >> maxLineLength))
		cerr << "Invalid number for maxLineLength" << argv[2] << '\n';

	vector< vector<int> > extraSpace(numWords+1, vector<int>(numWords + 1, 0));
	vector< vector<int> > penalty(numWords + 1, vector<int>(numWords + 1, 0));
	for (size_t i = 1; i <= numWords; i++) {
		extraSpace[i][i] = maxLineLength - wordLengths[i];
		for (size_t j = i + 1; j <= numWords; j++) {
			int a1 = extraSpace[i][j - 1];
			int a2 = wordLengths[j];
			extraSpace[i][j] = extraSpace[i][j - 1] - wordLengths[j] - 1;
		}
	}

	for (size_t i = 1; i <= numWords; i++) {
		for (size_t j = i; j <= numWords; j++) {
			if (extraSpace[i][j] < 0) {
				penalty[i][j] = INT_MAX;
			}
			else if (j == numWords && extraSpace[i][j] >= 0) {
				penalty[i][j] = 0;
			}
			else {
				penalty[i][j] = (int)pow(extraSpace[i][j], 3);
			}
		}
	}

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

	vector< vector<int> > prettLineIndices(numWords +1, vector<int>(2, 0));
	int num_pretty_lines = get_line_breaks(Prev, numWords, prettLineIndices);

	/*while(!num_pretty_lines){

	}*/
	file.close();
	return 0;
}