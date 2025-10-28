#include "plot.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <sstream>
#include <cmath>
#include <chrono>

using namespace std;

struct Interrupt {
	Interrupt() : interrupted(false) {};
	void continueSystem() {
		while(!interrupted) {
			//keep running
		}
	}
	atomic<bool> interrupted;
};

int numberCountThrough(vector<double> xCoords, vector<double> yCoords) {
	int sum = 0;
	for(int i = 0; i < xCoords.size(); i += 1) {
		sum += sqrt(pow(xCoords.at(i) - xCoords.at((i+1)%xCoords.size()), 2) + pow(yCoords.at(i) - yCoords.at((i+1)%yCoords.size()), 2));
	}
	return sum;
}

double normalize(string normalizeInput) {
    double base = stod(normalizeInput.substr(0,normalizeInput.find('e')+1));
    int exponent = stoi(normalizeInput.substr(normalizeInput.find('e')+1));
    if(exponent != 0)
        return base * pow(10,exponent);
    else 
        return base;
}

int main() {

	string inputData;
	string filename;
	ifstream inFS;
	ofstream outFS;
	vector<double> xCoords;
	vector<double> yCoords;

	cout << "Please input a filename: ";
	cin >> filename;

	inFS.open(filename);
	if (!inFS.is_open()) {
		cout << "Could not open file " << filename << endl;
		return 1;
	}

	getline(inFS,inputData);

	string xInput, yInput;

    while(!inFS.fail()) {
        stringstream ss(inputData);
        while(ss >> xInput >> yInput) {
            double normalizeXInput = normalize(xInput);
			double normalizeYInput = normalize(yInput);
            xCoords.push_back(normalizeXInput);
			yCoords.push_back(normalizeYInput);
        }
        getline(inFS,inputData);
    }

	inFS.close();
	
	cout << "There are " << xCoords.size() << " nodes, computing route..." << endl;
	cout << "Shortest Route Discovered So Far" << endl;

	int distance = 0;
	distance = numberCountThrough(xCoords, yCoords);
	cout << distance << endl;
	
	Interrupt interrupt;
	thread t(&Interrupt::continueSystem, &interrupt);
	cin.ignore(); //clear the newline character from the input buffer
	char c;


	while((c = getchar()) != '\n') {
		//checkFastestRoute();
	}
	interrupt.interrupted = true;
	t.join();

    signalsmith::plot::Plot2D plot;


	auto &line = plot.line();
	for (int x = 0; x < xCoords.size(); x += 1) {
        line.marker(xCoords.at(x),yCoords.at(x));
		line.add(xCoords.at(x),yCoords.at(x));
	}
	line.add(xCoords.at(0),yCoords.at(0));
	line.dot(xCoords.at(0),yCoords.at(0), 4,1);

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

	string svgFilename = filename + "_SOLUTION_" + to_string(distance) + ".svg";
	cout << "Route written to disk as " << svgFilename << endl;
	plot.write(svgFilename);	
}