#include "plot.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>

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

int numberCountThrough(vector<float> xCoords, vector<float> yCoords) {
	int sum = 0;
	for(int i = 0; i < xCoords.size(); i += 1) {
		sum += sqrt(pow(xCoords.at(i) - xCoords.at((i+1)%xCoords.size()), 2) + pow(yCoords.at(i) - yCoords.at((i+1)%yCoords.size()), 2));
	}
	return sum;
}

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
	cout << "Writing solution to " << filename + "_SOLUTION.txt" << endl;
	if (!outFS.is_open()) {
		cout << "Could not open file " << filename + "_SOLUTION.txt" << endl;
		return 1;
	}

	for(int i = 0; i < xCoords.size(); i += 1) {
		outFS << i+1 << endl;
	}
	outFS << "1" << endl;
	outFS.close();

	cout << "Visual representation for " << distance << " is in example.svg" << endl;
	plot.write("example.svg");	
}