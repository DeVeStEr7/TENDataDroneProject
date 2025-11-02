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
#include <conio.h>
#include <stdio.h>

using namespace std;

#ifdef _WIN32
    #include <conio.h>    // for _kbhit() and _getch() on Windows
#else
    #include <termios.h>  // for terminal control on macOS/Linux
    #include <unistd.h>   // for STDIN_FILENO
    #include <fcntl.h>    // for fcntl() non-blocking mode
#endif

bool enterPressed() {
#ifdef _WIN32
    if (_kbhit()) {            // check if a key was pressed
        char c = _getch();     // get the pressed key
        return c == '\r';      // '\r' is Enter on Windows
    }
    return false;
#else
    struct termios oldt, newt;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);         // get current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);       // disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0); // get current flags
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // non-blocking input

    int ch = getchar();                     // try reading a character

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore settings
    fcntl(STDIN_FILENO, F_SETFL, oldf);      // restore flags

    return ch == '\n';  // '\n' is Enter on Unix/macOS
#endif
}


// changes scientific notation to double
double normalize(string normalizeInput) {
    double base = stod(normalizeInput.substr(0,normalizeInput.find('e')+1));
    int exponent = stoi(normalizeInput.substr(normalizeInput.find('e')+1));
    if(exponent != 0)
        return base * pow(10,exponent);
    else 
        return base;
}

int main() {

	// variables initialization	
	string inputData;
	string filename;
	ifstream inFS;
	ofstream outFS;
	vector<double> xCoords;
	vector<double> yCoords;
	double p = 0.10;
	
	// process: files intak
	cout << "Please input a filename: ";
	cin >> filename;

	inFS.open(filename);

	// variable initialization
	double distance = 0.0;
	nearest_neighbor drone;
	drone.load_data(filename); // re reads info 
	distance = round(drone.nearest_neighbor_distance()*10)/10;

	// prints to UI
	cout << "There are " << drone.get_size() << " nodes, computing route..." << endl;
	cout << "	Shortest Route Discovered So Far" << endl;

	cout << "		" << distance << endl;
	double BSF = distance;
    
	cin.ignore();
	srand(time(NULL));
	while (true) {
    double new_distance = round(drone.modified_nearest_neighbor_distance(p)*10)/10;
    if (new_distance < BSF) {
        BSF = new_distance;
        cout << "        " << BSF << endl;
    }

    if (enterPressed()) break;  // ✅ non-blocking check for Enter key

   }

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

