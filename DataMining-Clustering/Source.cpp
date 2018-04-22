#include<iostream>
#include"lib/Reader.h"
#include"lib/ReaderHelper.h"
#include"lib/KMeans.h"
#include"lib/DBSCAN.h"
#include"lib/Spectral.h"
#include"lib/DenPeak.h"
using namespace std;

//string filePath = "dataset/lab1/datasets/";
//string fileName = "Aggregation_cluster=7.txt";
//string fileName = "flame_cluster=2.txt";
//string fileName = "Jain_cluster=2.txt";
//string fileName = "Pathbased_cluster=3.txt";
//string fileName = "Spiral_cluster=3.txt";
string outPath = "out/";

//my test
string filePath = "dataset/example/";
string fileName = "data.txt";

int main() {
	
	AroyaReader reader;
	reader.setSplit('\t');
	reader.read((filePath + fileName).c_str());
	AroyaReaderHelper helper;
	helper.insertAll(reader, false);

	AroyaKMeans kmeans;
	kmeans.setData(helper.getData());
	kmeans.setClusters(3);
	kmeans.setBord(0.01);
	kmeans.run();
	kmeans.writeFile((outPath + "KMeans - " + fileName).c_str(), true);

	AroyaDBSCAN dbscan;
	dbscan.setData(helper.getData());
	dbscan.setDensity(5);
	dbscan.setRadius(1);
	dbscan.run();
	dbscan.writeFile((outPath + "DBSCAN - " + fileName).c_str(), true);

	Spectral spectral;
	spectral.setData(helper.getData());
	spectral.myans.setClusters(3);
	spectral.myans.setBord(0.05);
	spectral.run();
	spectral.writeFile((outPath + "Spectral - " + fileName).c_str());

	DenPeak denpeak;
	denpeak.input(helper.getDataPointer(), helper.getRows(), helper.getColumns());
	denpeak.setMaximumDistance(1);
	denpeak.clustering();
	denpeak.writeFile(outPath + "DenPeak - " + fileName);

	//system("pause");
	return 0;
}