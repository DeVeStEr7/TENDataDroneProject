#include "plot.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
	int xInput, yInput;
	string filename;
	ifstream inFS;
	ofstream outFS;
	vector<float> xCoords;
	vector<float> yCoords;

	cout << "Please input a filename: ";
	cin >> filename;

	inFS.open(filename);
	if (!inFS.is_open()) {
		cout << "Could not open file " << filename << endl;
		return 1;
	}

	while(inFS >> xInput >> yInput) {
		xCoords.push_back(xInput);
		yCoords.push_back(yInput);
	}
	inFS.close();

	cout << "There are " << xCoords.size() << " nodes" << endl;
	cout << "Tracking fastest distance..." << endl;

    signalsmith::plot::Plot2D plot;


	auto &line = plot.line();
	for (int x = 0; x < xCoords.size(); x += 1) {
        line.marker(xCoords.at(x),yCoords.at(x));
		line.add(xCoords.at(x),yCoords.at(x));
	}

    //add creates the lines
    //marker makes the points

	outFS.open(filename + "_SOLUTION.txt");
	if (!outFS.is_open()) {
		cout << "Could not open file " << filename + "_SOLUTION.txt" << endl;
		return 1;
	}

	for(int i = 0; i < xCoords.size(); i += 1) {
		outFS << i+1 << endl;
	}
	outFS << "1" << endl;
	outFS.close();

	plot.write("example.svg");	
}