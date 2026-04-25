// How to compile:
// run g++ -std=c++17 main.cpp AirportGraph.cpp -o airport_graph
// then ./airport_graph airports.csv

#include "AirportGraph.hpp"
#include "Airport.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cctype>

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
            case 2: {
                string start, dest;
                cout << "Enter origin airport: ";
                cin >> start;
                cout << "Enter destination airport: ";
                cin >> dest;
                graph.shortestPath(start, dest);
                break;
            }
            
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
