#include "AirportGraph.hpp"
#include "Airport.tpp"
#include <fstream>
#include <regex>
#include <iostream>
#include <algorithm>

AirportGraph::AirportGraph() {}

int AirportGraph::getAirportIndex(const std::string& code) const {
    for (size_t i = 0; i < airportCodes.size(); ++i) {
        if (airportCodes[i] == code) return static_cast<int>(i);
    }
    return -1;
}

std::vector<std::string> AirportGraph::parseCSVLine(const std::string& line) const {
    vector<string> out; 
    regex del(",");
    
    // Create a regex_token_iterator to split the string
    sregex_token_iterator it(line.begin(), line.end(), del, -1);

    // End iterator for the regex_token_iterator
    sregex_token_iterator end;

    // Iterating through each token
    while (it != end) {
        string t = *it;
        if(t[0] == '\"'){
            t = t.substr(1, t.size() - 1);
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

void AirportGraph::buildGraphFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    airportCodes.clear();

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        auto fields = parseCSVLine(line);
        if (fields.size() < 6) continue;

        std::string origCode = fields[0];
        std::string destCode = fields[1];
        std::string origCity = fields[2];
        std::string destCity = fields[4];
        int dist = std::stoi(fields[6]);
        int cost = std::stoi(fields[7]);
        
        std::string origState = fields[3];
        std::string destState = fields[5];

        AirportNode origNode(origCode, origCity, origState);
        AirportNode destNode(destCode, destCity, destState);

        airportMap.insertAirport(origCode, origCity, origState);
        airportMap.insertAirport(destCode, destCity, destState);

        airportMap.insertRoute(origNode, destNode, dist, cost);

        if (std::find(airportCodes.begin(), airportCodes.end(), origCode) == airportCodes.end())
            airportCodes.push_back(origCode);
        if (std::find(airportCodes.begin(), airportCodes.end(), destCode) == airportCodes.end())
            airportCodes.push_back(destCode);
    }

    std::cout << "Graph built successfully using AirportMap: " 
              << airportMap.getNumAirports() << " airports loaded.\n";
    file.close();
}

void AirportGraph::displayAirportConnections() const {
    if (airportMap.getNumAirports() == 0) {
        std::cout << "Error: Graph not built yet.\n";
        return;
    }

    const auto& routes = airportMap.getRoutes();
    const auto& airports = airportMap.getAirports();
    int n = airportMap.getNumAirports();

    std::vector<std::pair<std::string, int>> connections;

    for (int i = 0; i < n; ++i) {
        int outCount = static_cast<int>(routes[i].size());
        int inCount = 0;
        for (int u = 0; u < n; ++u) {
            if (u == i) continue;
            for (const auto& r : routes[u]) {
                if (r.neighbor == airports[i]) ++inCount;
            }
        }
        int total = outCount + inCount;
        if (total > 0)
            connections.emplace_back(airports[i].getName(), total);
    }

    std::sort(connections.begin(), connections.end(),
              [](const auto& a, const auto& b){ return a.second > b.second; });

    std::cout << "\nTask 5: Airport Connectivity Count\n";
    std::cout << "Airport\tConnections\n";
    for (const auto& p : connections) {
        std::cout << p.first << "\t" << p.second << std::endl;
    }
}

void AirportGraph::buildUndirectedGraph() {
    if (airportMap.getNumAirports() == 0) {
        std::cout << "Error: Graph not built yet.\n";
        return;
    }

    const auto& routes = airportMap.getRoutes();
    int n = airportMap.getNumAirports();
    undirAdj.assign(n, std::vector<UndirectedEdge>());

    std::vector<std::vector<int>> minCost(n, std::vector<int>(n, INF));

    for (int u = 0; u < n; ++u) {
        for (const auto& r : routes[u]) {
            int v = airportMap.getAirportIndex(r.neighbor);
            if (v != -1)
                minCost[u][v] = std::min(minCost[u][v], r.cost);
        }
    }

    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            int c1 = minCost[u][v];
            int c2 = minCost[v][u];
            if (c1 < INF || c2 < INF) {
                int selectedCost = std::min(c1, c2);
                undirAdj[u].push_back({u, v, selectedCost});
                undirAdj[v].push_back({v, u, selectedCost});
            }
        }
    }
}

void AirportGraph::printUndirectedGraph() const {
    if (undirAdj.empty()) {
        std::cout << "Undirected graph has not been built yet.\n";
        return;
    }

    const auto& airports = airportMap.getAirports();
    std::cout << "\nTask 6: Undirected Graph Edges (u, v) cost\n";

    std::vector<std::tuple<std::string, std::string, int>> edges;
    int n = airportMap.getNumAirports();
    for (int u = 0; u < n; ++u) {
        for (const auto& e : undirAdj[u]) {
            if (u < e.to) {
                std::string a = airports[u].getName();
                std::string b = airports[e.to].getName();
                edges.emplace_back(a, b, e.cost);
            }
        }
    }

    std::sort(edges.begin(), edges.end());
    for (const auto& [u, v, cost] : edges) {
        std::cout << "(" << u << ", " << v << ")  cost: " << cost << std::endl;
    }
    std::cout << "\nTotal undirected edges created: " << edges.size() << std::endl;
}

void AirportGraph::minSpanningTree() {
    buildUndirectedGraph();
    int n = undirAdj.size();
    if (!n) {
        return;
    }

    const auto& airports = airportMap.getAirports();
    std::vector<std::string> tree;
    std::vector<bool> explored(n, false);
    PriorityQueue q;

    int exploredEdges = 0;
    int totalCost = 0;

    explored[0] = true;

    for (UndirectedEdge edge : undirAdj[0]) {
        q.push(edge);
    }
    

    while (!q.empty() && exploredEdges < n - 1) {
        UndirectedEdge edge = q.top();
        q.pop();

        int idx = edge.to;

        if (explored[idx]) {
            continue;
        }
        explored[idx] = true;
        totalCost += edge.cost;
        exploredEdges++;

        tree.push_back(airports[edge.from].getName() + " - " + airports[edge.to].getName() + " cost: " + to_string(edge.cost));
        for (const auto& push : undirAdj[idx]) {
            if (!explored[push.to]) {
                q.push(push);
            }
        }
    }

    if (exploredEdges != n - 1) {
        std::cout << "Not a connected graph, a MST cannot be made" << std::endl;
    } else {
        for (const auto& edge : tree) {
            std::cout << edge << std::endl; 
        }

        std::cout << "Total Cost: " << totalCost << std::endl;
    }

}

void AirportGraph::minSpanningForest() {
    buildUndirectedGraph();
    int n = undirAdj.size();
    if (!n) {
        return;
    }

    const auto& airports = airportMap.getAirports();
    std::vector<std::string> tree;
    PriorityQueue q;

    int exploredEdges = 0;
    int totalCost = 0;
    
    for (int i = 0; i < n; i++) {
        for (UndirectedEdge edge : undirAdj[i]) {
            q.push(edge);
        }
    }

    std::vector<std::vector<int>> mstAdj(n); 

    while (!q.empty()) {
        std::vector<bool> explored(n, false);
        UndirectedEdge edge = q.top();
        q.pop();

        if (!hasPath(edge.from, edge.to, mstAdj, explored)) {
            mstAdj[edge.from].push_back(edge.to);
            mstAdj[edge.to].push_back(edge.from);

            totalCost += edge.cost;
            exploredEdges++;

            tree.push_back(airports[edge.from].getName() + " - " + airports[edge.to].getName() + " cost: " + to_string(edge.cost));
        }
    }
    
    for (const auto& edge : tree) {
        std::cout << edge << std::endl; 
    }

    std::cout << "Total Cost: " << totalCost << std::endl;

}

bool AirportGraph::hasPath(int from, int to, std::vector<std::vector<int>>& mstAdj, std::vector<bool>& visited) {
    if (from == to) {
        return true;
    }

    visited[from] = true;

    for (int neighbor : mstAdj[from]) {
        if (!visited[neighbor]) {
            if (hasPath(neighbor, to, mstAdj, visited)) {
                return true;
            }
        }
    }

    return false;

}

void AirportGraph::shortestPath(const string& start, const string& dest) const{
    airportMap.shortestPath(start, dest);
    return;
}

void AirportGraph::shortestPathToState(const string& origin, const string& state) const{
    airportMap.shortestPathToState(origin, state);
    return;
}

void AirportGraph::shortestPathWithStops(const string& origin, const string& dest, int K) const{
    airportMap.shortestPathWithStops(origin, dest, K);
    return;
}
