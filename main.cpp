// How to compile:
// run g++ -std=c++17 main.cpp AirportGraph.cpp -o airport_graph
// then ./airport_graph airports.csv

#include "AirportGraph.hpp"
#include "Airport.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <stdexcept>
#include <cctype>

vector<string> split(const string& s);
void addRow(vector<string> words, AirportMap& m);
void runFromFile(const char* file_name, AirportMap& m);

int main(int argc, char* argv[]) {
    std::cout << "COP 3151 - Data Structures Team Project Spring 2026\n";
    std::cout << "Parts 5 and 6\n\n";

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " airports.csv\n";
        return 1;
    }

    AirportGraph graph;
    graph.buildGraphFromCSV(argv[1]);

    int choice = 0;
    while (choice != 9) {
        std::cout << "\nTask Chooser:\n";
        std::cout << "2) Find the shortest distance path between origin airport and destination airport (Task 2)\n";
        std::cout << "3) Find shortest by distance paths between a given origin airport to each airport located in a given destination state (Task 3)\n";
        std::cout << "4) Find the shortest by distance path between a given origin airport and destination airport with a specified number of stops (Task 4)\n";
        std::cout << "5) Display total direct flight connections (Task 5)\n";
        std::cout << "6) Create and print undirected graph (Task 6)\n";
        std::cout << "7) Create a minimum spanning tree using Prim's Algorithm\n";
        std::cout << "8) Create a minimum spanning forest using Kruskal's Algorithm\n";
        std::cout << "9) Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 2:
            
                break;
            case 3: {
                string origin, state;
                cout << "Enter origin airport: ";
                cin >> origin;
                cout << "Enter destination state (e.g., FL): ";
                cin >> state;
                graph.shortestPathToState(origin, state);
                break;
            }

            case 4: {
                string origin, dest;
                int stops;
                cout << "Enter origin airport: ";
                cin >> origin;
                cout << "Enter destination airport: ";
                cin >> dest;
                cout << "Enter number of stops: ";
                cin >> stops;
                graph.shortestPathWithStops(origin, dest, stops);
                break;
            }

            case 5:
                graph.displayAirportConnections();
                break;
            case 6:
                graph.buildUndirectedGraph();
                std::cout << "Task 6: Undirected graph created successfully";
                graph.printUndirectedGraph();
                break;
            case 7:
                graph.minSpanningTree();
                break;
            case 8:
                graph.minSpanningForest();
                break;
            case 9:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    }
    return 0;
}

vector<string> split(const string& s) {
    vector<string> out;
    regex del(",");
    // Create a regex_token_iterator to split the string
    sregex_token_iterator it(s.begin(), s.end(), del, -1);

    // End iterator for the 
    // regex_token_iterator
    sregex_token_iterator end;

    // Iterating through each token
    while (it != end) {
        string t = *it;
        if(t[0] == '\"'){
            t = t.substr(1);
            out.push_back(t);
        }
        else if(t[3] == '\"'){
            t = t.substr(1, 2);
            out.push_back(t);
        }
            
        else{
            out.push_back(*it);
        }
        ++it;
    }

    return out;
}


void runFromFile(const char* file_name, AirportMap& m) {
    ifstream in;

    // Open the file
    in.open(file_name);

    // Check if file exists
    if (in.fail()) {
        cerr << file_name << " could not be opened\n";
        throw logic_error("File could not be opened\n");
    }

    string line;
    vector<string> words;
    getline(in, line);
    while (getline(in, line)) {
        words = split(line);
        addRow(words, m);
    }

    // Close the file
    in.close();
}

void addRow(vector<string> words, AirportMap& m){
    if(!m.hasAirport(AirportNode(words[0], words[2], words[3]))){
        m.insertAirport(words[0], words[2], words[3]);
    }
    if(!m.hasAirport(AirportNode(words[1], words[4], words[5]))){
        m.insertAirport(words[1], words[4], words[5]);
    }
    
    m.insertRoute(AirportNode(words[0], words[2], words[3]), AirportNode(words[1], words[4], words[5]), stoi(words[6]), stoi(words[7]));
}
