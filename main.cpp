// How to compile:
// run g++ -std=c++17 main.cpp AirportGraph.cpp -o airport_graph
// then ./airport_graph airports.csv

#include "AirportGraph.hpp"
#include <iostream>
#include <string>

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
        std::cout << "5) Display total direct flight connections (Task 5)\n";
        std::cout << "6) Create and print undirected graph (Task 6)\n";
        std::cout << "9) Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 5:
                graph.displayAirportConnections();
                break;
            case 6:
                graph.buildUndirectedGraph();
                graph.printUndirectedGraph();
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