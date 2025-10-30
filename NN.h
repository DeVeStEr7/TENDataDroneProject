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
    const void get_route();  
    void load_data(const string &filename);
    double nearest_neighbor_distance();    
    double euclidean(int i, int j);
    void write_route_to_file(const string& filename);

    //void nearest_neighbor_route(double coordinates[][2], int n, const int route[]);
};   
const void nearest_neighbor::get_route()   {
    for(int i = 0; i < num_trees; ++i)  {
        cout << route[i] << endl;
    }
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

    for (int i = 0; i <= num_trees; ++i) { // include return to start
        int idx = route[i];
        fout << round(coordinates[idx][0]) << " " << round(coordinates[idx][1]) << endl;
    }
    
    fout.close();
    cout << "Route written to " << filename << endl;
}

    // double  nearest_neighbor::nearest_neighbor_distance()  {
    //     bool visited[256];
    //     double total_distance = 0.0;
        
    //     double charge_point[1][2] = {coordinates[0][0], coordinates[0][1]}; //starting point
    //     double curr_point[2] = {charge_point[0][0], charge_point[0][1]};
    //     visited[0] = true;

    //     int sprayed = 0;

    //     while(sprayed < num_trees - 1)  {
    //         double minDist = 1000000;
    //         int next_tree = -1;

    //         for(int i = 0; i < num_trees; ++i)  {
    //             if(!visited[i])  {
    //                 double dist = euclidean(curr_point, coordinates[i]);
    //                 if(dist < minDist)  {
    //                     minDist = dist;
    //                     next_tree = i;
    //                 }
    //             }
    //         }

    //         if(next_tree != -1)  {
    //             total_distance += minDist;
    //             curr_point[0] = coordinates[next_tree][0];
    //             curr_point[1] = coordinates[next_tree][1];
    //             visited[next_tree] = true;
    //             sprayed++;
    //         }
            
    //     }

    //     total_distance += euclidean(curr_point, charge_point[0]);   //return to the start
    //     return total_distance;
    // }        

    // void get_data(double graph_data[][2])   {
    //     if(num_trees < 1)   {
    //         cout << "No data loaded" << endl;
    //         return;
    //     }
        
    //     for(int i = 0; i < num_trees; ++i)  {
    //         coordinates[i][0] = graph_data[i][0];
    //         coordinates[i][1] = graph_data[i][1];
    //     }
    // }

    // double  nearest_neighbor::euclidean(int i, int j){
    //     double dx = coordinates[i][0] - coordinates[j][0];
    //     double dy = coordinates[i][1] - coordinates[j][1];
    //     return sqrt(dx*dx + dy*dy);
    // }
    
    // void  nearest_neighbor::nearest_neighbor_route(){
    //     bool visited[256] = {false};
    //     int current_tree = 0;
    //     route[0] = current_tree;
    //     visited[current] = true;
    //     for(int num_visited = 1; num_visited < num_trees; ++num_visited){
    //         int next_tree = -1;
    //         double minDistance = 1000000; 
    //         for (int j = 0; j < num_trees; ++j){
    //             if(!visited[j]){
    //                 double dist = euclidean(current_tree, j);
    //                 if(dist < minDistance){
    //                     minDistance = d;
    //                     next_tree = j;
    //                 }
    //             }
    //         }

    //     }
    //     route[num_visited] = next_tree;
    //     visited[next_tree] = tree;
    //     current_tree = next_tree;
    // }