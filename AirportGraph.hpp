#ifndef AIRPORT_GRAPH_HPP
#define AIRPORT_GRAPH_HPP

#include "Airport.hpp"
#include <vector>
#include <string>

class AirportGraph : public AirportMap {
private:
    AirportMap airportMap;                   

    struct UndirectedEdge {
        int from;
        int to;
        int cost;
    };
    std::vector<std::vector<UndirectedEdge>> undirAdj;
    std::vector<std::string> airportCodes;

    const int INF = 1000000005;

    int getAirportIndex(const std::string& code) const;
    std::vector<std::string> parseCSVLine(const std::string& line) const;

    class PriorityQueue { // helper Queue for algorithms, will handle sorting 
    public:
        std::vector<UndirectedEdge> data;


        void push(UndirectedEdge edge) {
            data.push_back(edge);
            if (data.size() > 1) {
                int index = data.size() - 1;
                while (index > 0 && data[index].cost < data[index - 1].cost) {
                    std::swap(data[index], data[index - 1]);
                    index--;
                }
            }
        }

        void pop() {
            data.erase(data.begin());
        }

        UndirectedEdge top() {
            return data[0];
        }

        bool empty() {
            return data.size() == 0;
        }
    };

public:
    AirportGraph();

    void buildGraphFromCSV(const std::string& filename);

    void displayAirportConnections() const;   // Task 5
    void buildUndirectedGraph();              // Task 6
    void printUndirectedGraph() const;

    void minSpanningTree(); // Task 7
    void minSpanningForest(); // Task 8

    bool hasPath(int from, int to, std::vector<std::vector<int>>& visitedGraph, std::vector<bool>& visited); // DFS approach for kruskals

    // Ensuring functions work with inheritance
    // Here to fix the problems inheritance causes them.
    void shortestPath(const string& start, const string& dest) const; // Task 2

    void shortestPathToState(const string& origin, const string& state) const; // Task 3

    void shortestPathWithStops(const string& origin, const string& dest, int K) const; // Task 4

};

#endif
