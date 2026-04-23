#ifndef AIRPORT_HPP
#define AIRPORT_HPP
#include <string>
#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>
#include <functional>

using namespace std;

class AirportNode { // Node that stores Airport name and city
private:
    string name; 
    string city;
    string state;

public:
    AirportNode(string n, string c, string s) : name(n), city(c), state(s) {}
    string getName(void) const { return name; }; //For retrivals of name while in AirportMap class
    string getCity(void) const { return city; }; //For retrivals of city while in AirportMap class
    string getState(void) const { return state; }; //For retrivals of state while in AirportMap class

    bool operator==(const AirportNode& other) const{
        return (this->name == other.name && this->city == other.city && this->state == other.state) ? true : false;
    }
    friend std::ostream& operator<<(std::ostream& os, const AirportNode& obj) {
        return os << "Airport Name: " << obj.name << ", City: " << obj.city << ", State: " << obj.state;
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

 
    bool hasRoute(int i1, const AirportNode& a2) const; // Checks if there's a direct flight between the airports (i1 is starting airport index, a2 is destination airport)

public:
    int getAirportIndex(const AirportNode& a) const; // Finds where Airport is located in airports vector
    void insertAirport(const string n, const string c, const string s); // Add new Airports to Map
    void insertRoute(const AirportNode& a1, const AirportNode& a2, int d = 1, int c = 1); // Makes a direct flight connection from one airport to another

    bool hasAirport(const AirportNode& a) const { return getAirportIndex(a) != -1 ? true : false; }; // Currently exists to help condense the text in the importing data phase
    void print() const; // Prints everything (remove later)

    // Task 3
    void shortestPathToState(const string& origin, const string& state);

    //Task 4
    void shortestPathWithStops(const string& origin, const string& dest, int K);

    int getNumAirports() const { return static_cast<int>(airports.size()); }
    const std::vector<AirportNode>& getAirports() const { return airports; }
    const std::vector<std::vector<Route>>& getRoutes() const { return routes; }
};

//#include "Airport.tpp"

#endif