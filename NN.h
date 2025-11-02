#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;



class nearest_neighbor{
   private:
        int num_trees;
        double coordinates[256][2];
        int route[256];
        double charge_point[1][2];
   
   public:
    nearest_neighbor() : num_trees(0) {}
    //void get_data(double graph_data[][2], int points); 
    int get_numTrees() const { return num_trees; }
    vector<int> get_route() const;  
    void load_data(const string &filename);
    double nearest_neighbor_distance();    
    double euclidean(int i, int j);
    void write_route_to_file(const string& filename);
    double modified_nearest_neighbor_distance(double p);
    //void nearest_neighbor_route(double coordinates[][2], int n, const int route[]);
};   
vector<int> nearest_neighbor::get_route() const  {
    return vector<int>(route, route + num_trees + 1);
}
void nearest_neighbor::load_data(const string &filename) {
        ifstream dataPoints;
        dataPoints.open(filename);
        if (!dataPoints) {
            cout << "Error with opening file " << endl;
            exit(1); 
        }
        
        num_trees = 0;
        string coordinate_line;
        while(getline(dataPoints, coordinate_line)) {
            stringstream ss(coordinate_line);
            double x, y;
            ss >> x >> y;
            coordinates[num_trees][0] = x;
            coordinates[num_trees][1] = y;
            num_trees++;
        }

        dataPoints.close();
        
    }        


// Euclidean distance between two points by index
double nearest_neighbor::euclidean(int i, int j) {
    double dx = coordinates[i][0] - coordinates[j][0];
    double dy = coordinates[i][1] - coordinates[j][1];
    return sqrt(dx * dx + dy * dy);
}

// Combined nearest neighbor distance calculation + route generation
double nearest_neighbor::nearest_neighbor_distance() {
    bool visited[256] = {false};
    double total_distance = 0.0;

    int current_tree = 0;      // start at the first coordinate
    route[0] = current_tree;
    visited[current_tree] = true;

    for (int i = 1; i < num_trees; ++i) {
        int next_tree = -1;
        double minDist = 100000;

        for (int j = 0; j < num_trees; ++j) {
            if (!visited[j]) {
                double dist = euclidean(current_tree, j);
                if (dist < minDist) {
                    minDist = dist;
                    next_tree = j;
                }
            }
        }

        if (next_tree != -1) {
            total_distance += minDist;
            route[i] = next_tree;
            visited[next_tree] = true;
            current_tree = next_tree;
        }
    }

    // Return to the start
    total_distance += euclidean(current_tree, 0);
    route[num_trees] = 0;
    return total_distance;
}

void nearest_neighbor::write_route_to_file(const string &filename) {
    ofstream fout(filename);
    if (!fout) {
        cerr << "Error: Could not write to " << filename << endl;
        return;
    }

    vector<int> r = get_route(); // get the route vector
    for (int node : r) {         // iterate over the vector
        fout << node +1 << endl;
    }
    
    fout.close();
    cout << "Route written to disk as " << filename << endl;
}




double nearest_neighbor::modified_nearest_neighbor_distance(double p) {
    bool visited[256] = {false};
    double total_distance = 0.0;
    int current_tree = 0;
    int temp_route[256];
    temp_route[0] = current_tree;
    visited[current_tree] = true;

    for (int i = 1; i < num_trees; ++i) {
        int bestNextTree1 = -1;  //Indices of the 2 closest trees
        int bestNextTree2 = -1;

        double bestDistance = 100000;  //Distance to the 2 closest trees
        double bestDistance2nd = 100000;

        for (int j = 0; j < num_trees; ++j) {
            if (!visited[j]) {
                double d = euclidean(current_tree, j);
                if (d < bestDistance) {
                    //If we find a closer tree we mpve the current best distance to the 2nd best distance 
                    bestDistance2nd = bestDistance; 
                    bestNextTree2 = bestNextTree1;
                    //and update the best distance
                    bestDistance = d; 
                    bestNextTree1 = j;
                } else if (d < bestDistance2nd) {
                    bestDistance2nd = d; 
                    bestNextTree2 = j;
                }
            }
        }

        int next_tree = bestNextTree1;
        if (bestNextTree2 && ((double)rand() / RAND_MAX) < p)
            next_tree = bestNextTree2;

        total_distance += euclidean(current_tree, next_tree);
        temp_route[i] = next_tree;
        visited[next_tree] = true;
        current_tree = next_tree;
    }

    total_distance += euclidean(current_tree, 0);
    temp_route[num_trees] = 0;

    for (int i = 0; i <= num_trees; ++i){
        route[i] = temp_route[i];
    }
    
    return total_distance;
}
