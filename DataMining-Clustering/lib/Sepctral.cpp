
#include"Spectral.h"
#include<cmath>
#include<stack>

#include"Writer.h"
#include<iostream>
using namespace std;

Spectral::Spectral() {
	spectraltype = RatioCut;
	dimension = 1;
	//to do
	maxDistance = DBL_MAX;
}

void Spectral::setData(const vector<vector<double>>& newData)
{
	origin = newData;
	int i, j, k;
	double temp, tri;
	int rows = newData.size();
	int columns = newData[0].size();
	//init data space
	data.resize(rows, columns);
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			data(i, j) = newData[i][j];
		}
	}
	flag.resize(rows);

	//caculate all distances
	MatrixXd distances;
	distances.resize(rows, rows);
	for (i = 0; i < rows; i++) {
		tri = 0;
		for (j = i + i; j < rows; j++) {
			temp = 0;
			for (k = 0; k < columns; k++) {
				temp += pow(data(i, k) - data(j, k), 2);
			}
			distances(i, j) = temp;
			distances(j, i) = temp;
			tri += temp;
		}
		distances(i, i) = tri;
	}

	//RBF data process
	//first caculate the maximum distance
	double maxDis = DBL_MIN;
	for (i = 0; i < rows; i++) {
		for (j = i + 1; j < rows; j++) {
			if (maxDis < distances(i, j))maxDis = distances(i, j);
		}
	}
	maxDistance = sqrt(maxDis);
	maxDis = maxDis*maxDis * 2;
	//generate RBF Matrix & exchange to Laplace Matrix
	MatrixXd RBF;
	RBF.resize(rows, rows);
	for (i = 0; i < rows; i++) {
		for (j = i + i; j < rows; j++) {
			temp = -exp(-distances(i,j) / maxDis);
			RBF(i, j) = temp;
			RBF(j, i) = temp;
		}
		RBF(i, i) = rows;
	}
	data = RBF;
}

void Spectral::run()
{
	double temp;
	int i, j, k,  rows, cols;
	
	//only support RatioCut now
	assert(spectraltype == RatioCut);
	//get EigenVectors
	EigenSolver<MatrixXd> Hsolver(data);
	MatrixXcd eigenVectors = Hsolver.eigenvectors();
	MatrixXcd eigenValues = Hsolver.eigenvalues();
	//find k minimum eigenvectors to dimension reduction
	stack<int>indexBase;
	stack<int>tempBase;
	rows = eigenVectors.rows();
	cols = eigenVectors.cols();
	indexBase.push(0);
	for (i = 1; i < rows; i++) {
		while (!indexBase.empty() && eigenValues(i, 0).real != 0 && 
			eigenValues(indexBase.top(), 0).real() > eigenValues(i, 0).real())
		{
			tempBase.push(indexBase.top());
			indexBase.pop();
		}
		indexBase.push(i);
		while (!tempBase.empty()) {
			indexBase.push(tempBase.top());
			tempBase.pop();
		}
		while(indexBase.size()>dimension){
			indexBase.pop();
		}
	}
	//select the vectors
	MatrixXd H(rows, dimension);
	for (i = 0; i < dimension; i++) {
		k = indexBase.top();
		indexBase.pop();
		for (j = 0; j < rows; j++) {
			H(j, i) = eigenVectors(j, k).real();
		}
	}
	//normalization
	rows = H.rows();
	cols = H.cols();
	//for (i = 0; i < rows; i++) {
	//	temp = 0;
	//	for (j = 0; j < cols; j++) {
	//		temp += pow(H(i, j),2);
	//	}
	//	temp = sqrt(temp);
	//	for (j = 0; j < cols; j++) {
	//		H(i, j) /= temp;
	//	}
	//}
	//cout << H << endl;
	//use other algorithm to cluster
	vector<vector<double>>transfer;
	vector<double>empty;
	for (i = 0; i < rows; i++) {
		transfer.push_back(empty);
		for (j = 0; j < cols; j++) {
			transfer[i].push_back(H(i, j));
		}
	}
	//AroyaDBSCAN myans;
	//myans.setRadius(maxDistance / rows/200.0);
	//myans.setDensity(5);
	//myans.setData(transfer);
	//myans.run();
	
	myans.setData(transfer);
	myans.run();
	flag = myans.getFlag();
}
void Spectral::writeFile(const char *fileName)
{
	::writeFile(origin, flag, fileName);
}