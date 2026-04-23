//How to run
// compile with: g++ -std=c++17 main.cpp AirportGraph.cpp -o airport_graph
// run with: ./airport_graph airports.csv


#include "AirportGraph.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    std::cout << "COP 3151 - Data Structures Team Project Spring 2026\n";
    std::cout << "Parts 5 and 6\n";

    if (argc < 2) {
        std::cerr << "missisng csv file.\n";
        std::cerr << "usage: " << argv[0] << " airports.csv\n";
        return 1;
    }

    std::string csvFile = argv[1];

    AirportGraph graph;

    graph.buildGraphFromCSV(csvFile);

    int choice = 0;
    while (choice != 9) {
        std::cout << "Task Chooser: enter the exact number for desired task.\n";
        std::cout << "5) Display total direct flight connections for each airport (Task 5)\n";
        std::cout << "6) Create undirected graph (Task 6)\n";
        std::cout << "9) Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 5:
                std::cout << "\n Task 5 - Airport Connectivity Count\n";
                graph.displayAirportConnections();
                std::cout << "\n";
                break;

            case 6:
                std::cout << "\n Task 6 - Undirected Graph Construction\n";
                graph.buildUndirectedGraph();
                graph.printUndirectedGraph(); 
                break;

            case 9:
                std::cout << "Exiting program.\n";
                break;

            default:
                std::cout << "Invalid option.\n\n";
                break;
        }
    }

    return 0;
}
