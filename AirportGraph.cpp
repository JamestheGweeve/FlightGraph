#include "AirportGraph.hpp"
#include "Airport.tpp"
#include <fstream>
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
    std::vector<std::string> result;
    std::string field;
    bool inQuotes = false;
    for (char c : line) {
        if (c == '"') inQuotes = !inQuotes;
        else if (c == ',' && !inQuotes) {
            result.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    result.push_back(field);
    return result;
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
        std::string destCity = fields[3];
        int dist = std::stoi(fields[4]);
        int cost = std::stoi(fields[5]);

        std::string origState = (origCity.find(", ") != std::string::npos) 
                              ? origCity.substr(origCity.find(", ") + 2) : "";
        std::string destState = (destCity.find(", ") != std::string::npos) 
                              ? destCity.substr(destCity.find(", ") + 2) : "";

        AirportNode origNode(origCode, origCity, origState);
        AirportNode destNode(destCode, destCity, destState);

        if (!airportMap.hasAirport(origNode))
            airportMap.insertAirport(origCode, origCity, origState);
        if (!airportMap.hasAirport(destNode))
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
                undirAdj[u].push_back({v, selectedCost});
                undirAdj[v].push_back({u, selectedCost});
            }
        }
    }

    std::cout << "\nTask 6: Undirected graph created successfully (" << n << " airports).\n";
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

void AirportGraph::shortestPathToState(const std::string& origin, const std::string& state) {
    airportMap.shortestPathToState(origin, state);
}

void AirportGraph::shortestPathWithStops(const std::string& origin, const std::string& dest, int K) {
    airportMap.shortestPathWithStops(origin, dest, K);
}