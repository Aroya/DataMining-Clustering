#include"DBSCAN.h"

#include<iostream>
#include<cmath>
#include<fstream>
#include<stack>
using namespace std;

AroyaDBSCAN::AroyaDBSCAN()
{
	radius = 2;
	density = 5;
	data = nullptr;
	rows = 0;
	columns = 0;
	clusters = 0;
	cluster = nullptr;

}

void AroyaDBSCAN::setData(const vector<vector<double>>&newData)
{
	if (data != nullptr) {
		for (int i = 0; i < rows; i++) {
			delete[]data[i];
		}
		delete[]data;
	}
	data = nullptr;
	rows = newData.size();
	if (rows < 1) {
		cout << "AroyaDBSCAN::setData() get empty data!\n";
		columns = 0;
		system("pause");
		return;
	}
	columns = newData[0].size();
	data = new double*[rows];
	for (int i = 0; i < rows; i++) {
		data[i] = new double[columns];
		for (int j = 0; j < columns; j++) {
			data[i][j] = newData[i][j];
		}
	}
	clusters = 0;
}

void AroyaDBSCAN::setRadius(const double &newRadius)
{
	radius = newRadius*newRadius;
}

void AroyaDBSCAN::setDensity(const int &newDensity)
{
	density = newDensity;
}

void AroyaDBSCAN::run()
{
	int i, j, k, l;
	//初始化空间
	if (cluster != nullptr) {
		delete[]cluster;
	}
	cluster = new int[rows];
	for (i = 0; i < rows; i++) {
		cluster[i] = -2;
	}
	//-2:unsign
	//-1:noise
	//0~:cluster

	double **distance = new double*[rows];
	//symmetric matrix, use triangular matrix to store
	double temp_dis;
	for (i = 0; i < rows; i++) {
		distance[i] = new double[i];
		for (j = 0; j < i; j++) {
			temp_dis = 0;
			for (k = 0; k < columns; k++) {
				temp_dis += pow(data[i][k] - data[j][k], 2);
			}
			distance[i][j] = temp_dis;
		}
	}
	
	//开始聚类
	stack<int>base;
	stack<int>base_base;
	stack<int>base_index;
	for (i = 0; i < rows; i++) {
		if (cluster[i] != -2) {
			continue;
		}
		//为-2时
		base.push(i);
		base_index.push(-1);
		while (!base.empty()) {
			//if base_index.top is not -1, already judged core point
			l = base.top();
			k = base_index.top();
			if (k != -1) {
				for (; k < l; k++) {
					if (distance[l][k] < radius&&cluster[k] < 0) {
						base_index.top() = k;
						base_index.push(-1);
						base_base.push(l);
						base.push(k);
						break;
					}
				}
				if (k < l)continue;
				for (k = l + 1; k < rows; k++) {
					if (distance[k][l] < radius&&cluster[k] < 0) {
						base_index.top() = k;
						base_index.push(-1);
						base_base.push(l);
						base.push(k);
						break;
					}
				}
				if (k == rows) {
					base.pop();
					if (!base_base.empty())base_base.pop();
					base_index.pop();
				}
			}
			else {
				//judge is core point
				//caculate density first
				int thisDensity = 0;
				for (j = 0; j < l; j++) {
					if (distance[l][j] < radius) {
						thisDensity++;
					}
				}
				for (j = l + 1; j < rows; j++) {
					if (distance[j][l] < radius) {
						thisDensity++;
					}
				}
				

				//not core point, border point
				if (thisDensity < density) {
					//no parents → noise point
					if (base_base.empty()) {
						cluster[l] = -1;
					}
					else {//extend parent's cluster
						cluster[l] = cluster[base_base.top()];
						base_base.pop();
					}
					base.pop();
					base_index.pop();
				}
				else {//core point
					if (base_base.empty()) {
						cluster[l] = ++clusters;
					}
					else {//extend parent's cluster
						cluster[l] = cluster[base_base.top()];
					}
					base_index.top() = 0;
				}
			}
		}
	}

	//clean dynamic memories
	for (i = 0; i < rows; i++) {
		delete[]distance[i];
	}
	delete[]distance;
}

void AroyaDBSCAN::writeFile(const char * fileName, const bool&withData) {
	ofstream fout;
	fout.open(fileName);
	int j;
	if (withData) for (int i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			fout << data[i][j] << ',';
		}
		fout << cluster[i] << endl;
	}
	else for (int i = 0; i < rows; i++) {
		fout << cluster[i] << endl;
	}
	fout.close();
}
vector<int> AroyaDBSCAN::getFlag()
{
	vector<int>ans;
	for (int i = 0; i < rows; i++)ans.push_back(cluster[i]);
	return ans;
}