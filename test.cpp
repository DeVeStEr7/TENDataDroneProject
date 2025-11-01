#include "plot.h"
#include "NN.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <atomic>
#include <sstream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;

// struct Interrupt {
// 	Interrupt() : interrupted(false) {};
// 	void continueSystem() {
// 		while(!interrupted) {
// 			//keep running
// 		}
// 	}
// 	atomic<bool> interrupted;
// };

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
	double p = 0.10;
	
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
	cout << "	Shortest Route Discovered So Far" << endl;

	double distance = 0.0;
	nearest_neighbor drone;
	drone.load_data(filename);
	distance = round(drone.nearest_neighbor_distance()*10)/10;
	cout << "		" << distance << endl;
	double BSF = distance;
    
	cin.ignore();
	srand(time(NULL));
	while(true){
		double new_distance = round(drone.modified_nearest_neighbor_distance(p)*10)/10;
		if(new_distance < BSF){
			BSF = new_distance;
			cout << "		" << BSF << endl;
		}
		if(cin.peek() == '\n'){
			cin.get();
			break;
		}
	}
	

	// Interrupt interrupt;
	// thread t(&Interrupt::continueSystem, &interrupt);
	//cin.ignore(); //clear the newline character from the input buffer
	//char c;


	// while((c = getchar()) != '\n') {
	// 	//checkFastestRoute();
	// }
	// interrupt.interrupted = true;
	// t.join();

	ostringstream dist;
    dist << fixed << setprecision(0) << BSF;
	string outputFilename = filename + "_SOLUTION_" + dist.str() + ".txt";
	drone.write_route_to_file(outputFilename);

    signalsmith::plot::Plot2D plot;
    vector<int> route = drone.get_route();

    
	auto &line = plot.line();
	for (int x = 0; x < route.size(); ++x) {
        int i = route[x];
        line.marker(xCoords[route[i]],yCoords[route[i]]);
		line.add(xCoords[route[i]],yCoords[route[i]]);
	}
	line.add(xCoords[route[0]],yCoords[route[0]]);
	line.dot(xCoords[route[0]],yCoords[route[0]], 4,1);

    //add creates the lines
    //marker makes the points

	string svgFilename = filename + "_SOLUTION_" + dist.str() + ".svg";
	plot.write(svgFilename);	
}

