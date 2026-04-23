#ifndef AIRPORT_HPP
#define AIRPORT_HPP
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class AirportNode { // Node that stores Airport name and city
private:
    string name; 
    string city;

public:
    AirportNode(string n, string c) : name(n), city(c) {}
    string getName(void) const { return name; };

    bool operator==(const AirportNode& other) const{
        return (this->name == other.name && this->city == other.city) ? true : false;
    }
    friend std::ostream& operator<<(std::ostream& os, const AirportNode& obj) {
        return os << "Airport Name: " << obj.name << ", City: " << obj.city;
    }
}; 

class Route { // The Edge, has weights distance and cost
public:
    AirportNode neighbor; // Refers to the Destination Airport
    int distance; // weight 1
    int cost; // weight 2

    Route(AirportNode n, int d, int c) : neighbor(n), distance(d), cost(c) {}
};

class AirportMap { // The Graph
private:
    vector<AirportNode> airports; // vertices of graph
    vector<vector<Route>> routes; // edges

    int getAirportIndex(const AirportNode& a) const; // Finds where Airport is located in airports vector
    bool hasRoute(int i1, const AirportNode& a2) const; // Checks if there's a direct flight between the airports (i1 is starting airport index, a2 is destination airport)

public:
    void insertAirport(const string n, const string c); // Add new Airports to Map
    void insertRoute(const AirportNode& a1, const AirportNode& a2, int d = 1, int c = 1); // Makes a direct flight connection from one airport to another

    bool hasAirport(const AirportNode& a) const { return getAirportIndex(a) != -1 ? true : false; }; // Currently exists to help condense the text in the importing data phase
    void print() const; // Prints everything (remove later)

};

#include "Airport.tpp"

#endif