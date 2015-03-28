#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool areSameAt(int nameIdx, vector<string>& files)
{
	int numOfFile = files.size();
	bool areSame = true;
	for(int fileIdx = 1; fileIdx < numOfFile; fileIdx++) {
		string& preFile = files[fileIdx-1];
		string& curFile = files[fileIdx];
		if(preFile[nameIdx] != curFile[nameIdx]) {
			areSame = false;
			break;
		}
	}
	return areSame;
}

int main()
{
	int numOfFile;
	cin >> numOfFile;
	vector<string> files(numOfFile);
	for(int i=0; i<numOfFile; i++)
		cin >> files[i];

	int nameLen = files[0].size();
	string pattern(nameLen, '\0');
	for(int nameIdx = 0; nameIdx < nameLen; nameIdx++) {
		if(areSameAt(nameIdx, files))
			pattern[nameIdx] = files[0][nameIdx];
		else
			pattern[nameIdx] = '?';
	}
	cout << pattern << endl;
}