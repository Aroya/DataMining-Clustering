
#include"Writer.h"

#include<fstream>
using namespace std;
void writeFile(const vector<vector<double>>& data, const vector<int>& flag, const char *& fileName)
{
	int rows = data.size();
	int columns = data[0].size();
	ofstream fout;
	fout.open(fileName);
	int j;
	for (int i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			fout << data[i][j] << ',';
		}
		fout << flag[i] << endl;
	}
	fout.close();
}