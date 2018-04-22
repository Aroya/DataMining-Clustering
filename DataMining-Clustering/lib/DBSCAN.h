#ifndef AROYA_DBSCAN
#define AROYA_DBSCAN

#include<vector>
using namespace std;

class AroyaDBSCAN {
public:
	AroyaDBSCAN();
	void setData(const vector<vector<double>>&newData);
	void setRadius(const double&);
	void setDensity(const int&);
	void run();
	vector<int>getFlag();
	void writeFile(const char*fileName, const bool&withData = false);
private:
	//maximum radius
	double radius;
	//minimum density
	int density;

	//data
	double**data;
	int rows, columns, clusters;

	//点的分类
	int *cluster;

	//distance实时计算
};

#endif