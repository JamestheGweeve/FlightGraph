#include "AirportGraph.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

AirportGraph::AirportGraph() {}

int AirportGraph::getAirportIndex(const std::string& code) const {
    for (int i = 0; i < numAirports; ++i) {
        if (airportCodes[i] == code) return i;
    }
    return -1;
}

std::vector<std::string> AirportGraph::parseCSVLine(const std::string& line) const {
    std::vector<std::string> result;
    std::string field;
    bool inQuotes = false;
    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
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
    std::getline(file, line);

    std::vector<std::pair<std::string, std::string>> rawEdges;
    std::vector<int> rawDist, rawCost;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        auto fields = parseCSVLine(line);
        if (fields.size() != 6) continue;

        std::string orig = fields[0];
        std::string dest = fields[1];
        int dist = std::stoi(fields[4]);
        int cost = std::stoi(fields[5]);

        rawEdges.emplace_back(orig, dest);
        rawDist.push_back(dist);
        rawCost.push_back(cost);

        bool foundOrig = false, foundDest = false;
        for (const auto& code : airportCodes) {
            if (code == orig) foundOrig = true;
            if (code == dest) foundDest = true;
        }
        if (!foundOrig) airportCodes.push_back(orig);
        if (!foundDest) airportCodes.push_back(dest);
    }

    numAirports = static_cast<int>(airportCodes.size());
    adj.resize(numAirports);

    for (size_t i = 0; i < rawEdges.size(); ++i) {
        int u = getAirportIndex(rawEdges[i].first);
        int v = getAirportIndex(rawEdges[i].second);
        if (u != -1 && v != -1) {
            adj[u].push_back({v, rawDist[i], rawCost[i]});
        }
    }

    std::cout << "Graph based on data: " << numAirports << " airports, " << rawEdges.size() << " directed flights." << std::endl;
    file.close();
}

void AirportGraph::displayAirportConnections() const {
    if (numAirports == 0) {
        std::cout << "Error: Graph not built yet. Call buildGraphFromCSV first." << std::endl;
        return;
    }

    std::vector<int> outDeg(numAirports, 0);
    std::vector<int> inDeg(numAirports, 0);

    for (int u = 0; u < numAirports; ++u) {
        outDeg[u] = static_cast<int>(adj[u].size());
        for (const auto& e : adj[u]) {
            if (e.to >= 0 && e.to < numAirports) {
                inDeg[e.to]++;
            }
        }
    }

    struct AirportConn {
        std::string code;
        int total;
    };
    std::vector<AirportConn> connections;
    for (int i = 0; i < numAirports; ++i) {
        connections.push_back({airportCodes[i], outDeg[i] + inDeg[i]});
    }

    std::sort(connections.begin(), connections.end(),
              [](const AirportConn& a, const AirportConn& b) {
                  return a.total > b.total;
              });

    std::cout << "Airport\tConnections" << std::endl;
    for (const auto& conn : connections) {
        if (conn.total > 0) {
            std::cout << conn.code << "\t" << conn.total << std::endl;
        }
    }
}

void AirportGraph::buildUndirectedGraph() {
    if (numAirports == 0) {
        std::cout << "Error: Graph not built yet." << std::endl;
        return;
    }

    undirAdj.assign(numAirports, std::vector<UndirectedEdge>());

    std::vector<std::vector<int>> minCost(numAirports, std::vector<int>(numAirports, INF));
    for (int u = 0; u < numAirports; ++u) {
        for (const auto& e : adj[u]) {
            int v = e.to;
            if (v >= 0 && v < numAirports) {
                minCost[u][v] = std::min(minCost[u][v], e.cost);
            }
        }
    }

    for (int u = 0; u < numAirports; ++u) {
        for (int v = u + 1; v < numAirports; ++v) {
            int c1 = minCost[u][v];
            int c2 = minCost[v][u];
            if (c1 < INF || c2 < INF) {
                int selectedCost = std::min(c1, c2);
                undirAdj[u].push_back({v, selectedCost});
                undirAdj[v].push_back({u, selectedCost});
            }
        }
    }

    std::cout << "Undirected graph created successfully (" << numAirports << " airports)." << std::endl;
}

void AirportGraph::printUndirectedGraph() const {
    if (numAirports == 0 || undirAdj.empty()) {
        std::cout << "Undirected graph has not been built yet." << std::endl;
        return;
    }

    std::cout << "\nTask 6: Undirected Graph Edges (u, v)" << std::endl;

    std::vector<std::tuple<std::string, std::string, int>> edges;

    for (int u = 0; u < numAirports; ++u) {
        for (const auto& e : undirAdj[u]) {
            std::string a = airportCodes[u];
            std::string b = airportCodes[e.to];
            if (a > b) std::swap(a, b);
            edges.emplace_back(a, b, e.cost);
        }
    }

    std::sort(edges.begin(), edges.end());
    auto last = std::unique(edges.begin(), edges.end());
    edges.erase(last, edges.end());

    for (const auto& [u, v, cost] : edges) {
        std::cout << "(" << u << ", " << v << ")  cost: " << cost << std::endl;
    }

    std::cout << "\nTotal undirected edges created: " << edges.size() << std::endl;
}
