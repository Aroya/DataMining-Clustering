#ifndef AROYA_SPECTRAL
#define AROYA_SEPCTRAL


#include"eigen3/Eigen/Dense"
using namespace Eigen;
#include<vector>
#include"KMeans.h"
using namespace std;

enum SpectralType{RatioCut};

class Spectral {
public:
	Spectral();
	void setData(const vector<vector<double>>&newData);

	void run();
	void writeFile(const char*fileName);
	//clustering method
	AroyaKMeans myans;
private:
	//store origin data
	MatrixXd data;
	//store clustering ans
	vector<vector<double>>origin;
	vector<int>flag;
	//setting of Spectral Type
	SpectralType spectraltype;
	//anti-span the dimension
	int dimension;
	//record
	double maxDistance;

};


#endif