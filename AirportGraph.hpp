#ifndef AIRPORT_GRAPH_HPP
#define AIRPORT_GRAPH_HPP

#include "Airport.hpp"
#include <vector>
#include <string>

class AirportGraph {
private:
    AirportMap airportMap;                   

    struct UndirectedEdge {
        int to;
        int cost;
    };
    std::vector<std::vector<UndirectedEdge>> undirAdj;
    std::vector<std::string> airportCodes;

    const int INF = 1000000005;

    int getAirportIndex(const std::string& code) const;
    std::vector<std::string> parseCSVLine(const std::string& line) const;

public:
    AirportGraph();

    void buildGraphFromCSV(const std::string& filename);
    void shortestPathToState(const std::string& origin, const std::string& state);  // Task 3 wrapper
    void shortestPathWithStops(const std::string& origin, const std::string& dest, int K);  // Task 4 wrapper
    void displayAirportConnections() const;   // Task 5
    void buildUndirectedGraph();              // Task 6
    void printUndirectedGraph() const;
};

#endif