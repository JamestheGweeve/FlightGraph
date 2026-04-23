#ifndef AIRPORT_GRAPH_HPP
#define AIRPORT_GRAPH_HPP

#include <vector>
#include <string>

class AirportGraph {
private:
    struct DirectedEdge {
        int to;
        int distance;
        int cost;
    };

    struct UndirectedEdge {
        int to;
        int cost;
    };

    std::vector<std::vector<DirectedEdge>> adj;
    std::vector<std::vector<UndirectedEdge>> undirAdj;
    std::vector<std::string> airportCodes;
    int numAirports = 0;

    const int INF = 1000000005;

    int getAirportIndex(const std::string& code) const;

    std::vector<std::string> parseCSVLine(const std::string& line) const;

public:
    AirportGraph();

    void buildGraphFromCSV(const std::string& filename);

    void displayAirportConnections() const;

    void buildUndirectedGraph();

    void printUndirectedGraph() const;

    const std::vector<std::string>& getAirportCodes() const { return airportCodes; }
    int getNumAirports() const { return numAirports; }
    const std::vector<std::vector<DirectedEdge>>& getDirectedAdj() const { return adj; }
    const std::vector<std::vector<UndirectedEdge>>& getUndirectedAdj() const { return undirAdj; }
};

#endif