#include "Airport.hpp"

// AirportMap Methods

int AirportMap::getAirportIndex(const AirportNode& a) const{
    for(int i = 0; i < airports.size(); i++){
        if(airports[i] == a){
            return i;
        }
    }
    return -1;
}

bool AirportMap::hasRoute(int i1, const AirportNode& a2) const{
    if (i1 < 0 || i1 >= routes.size()){
        return false;
    }

    for(const Route& r : routes[i1]){
        if(r.neighbor == a2){
            return true;
        }
    }

    return false;
}

void AirportMap::insertAirport(const string n, const string c, const string s){
    AirportNode a(n, c, s);
    if(getAirportIndex(a) != -1){
        cout << "Airport already added\n";
        return;
    }
    
    airports.push_back(a);
    vector<Route> t;
    routes.push_back(t);
}

void AirportMap::insertRoute(const AirportNode& a1, const AirportNode& a2, int d, int c){
    int idx1 = getAirportIndex(a1);
    int idx2 = getAirportIndex(a2);
    if(idx1 == -1 || idx2 == -1){
        cout << "One or both don't exist" << endl;
        return;
    }

    if(!hasRoute(idx1, a2)) {
        routes[idx1].push_back(Route(a2,d,c));
    }
}

void AirportMap::print(void) const{ // Was made for the purposes of testing out the import code
    for (int i = 0; i < airports.size(); i++) {
        cout << "{ " << airports[i].getName() << ": ";
        for(int j = 0; j < routes[i].size(); j++) {
            cout << "(" << routes[i][j].neighbor << ", " << routes[i][j].distance << ", " << routes[i][j].cost << ") ";
        }
        cout << "}\n";
    }
}

void AirportMap::shortestPathToState(const string& origin, const string& state){
    int n = airports.size();
    int src = getAirportIndex(AirportNode(origin, "", ""));

    if (src == -1){
        cout << "Origin not found\n";
        return;
    }

    vector<int> dist(n, INT_MAX);
    vector<int> cost(n, INT_MAX);
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);

    dist[src] = 0;
    cost[src] = 0;

    // Dijkstra
    for(int i = 0; i < n; i++){
        int u = -1;
        for(int j = 0; j < n; j++){
            if(!visited[j] && (u == -1 || dist[j] < dist[u])){
                u = j;
            }
        }

        if(dist[u] == INT_MAX) break;

        visited[u] = true;

        for(const Route& r : routes[u]){
            int v = getAirportIndex(r.neighbor);
            if(dist[u] + r.distance < dist[v]){
                dist[v] = dist[u] + r.distance;
                cost[v] = cost[u] + r.cost;
                parent[v] = u;
            }
        }
    }

    // Output
    cout << "Shortest paths from " << origin << " to " << state << " state airports are:\n\n";
    cout << "Path\tLength\tCost\n";

    bool found = false;

    for(int i = 0; i < n; i++){
        if(airports[i].getState() == state && dist[i] != INT_MAX){
            found = true;

            vector<string> path;
            for(int v = i; v != -1; v = parent[v]){
                path.push_back(airports[v].getName());
            }

            reverse(path.begin(), path.end());

            // Print path in format
            for(int j = 0; j < path.size(); j++){
                cout << path[j];
                if(j != path.size() - 1){
                    cout << "->";
                }   
            }

            cout << "\t" << dist[i] << "\t" << cost[i] << endl;

        }
    }
    if(!found){
        cout << "No paths found\n";
    }
}

void AirportMap::shortestPathWithStops(const string& origin, const string& dest, int K){
    int n = airports.size();
    int src = getAirportIndex(AirportNode(origin, "", ""));
    int target = getAirportIndex(AirportNode(dest, "", ""));

    if (src == -1 || target == -1){
        cout << "Invalid airports\n";
        return;
    }

    int bestDist = INT_MAX;
    int bestCost = 0;

    vector<int> bestPath;
    vector<int> path;
    vector<bool> visited(n, false);

    function<void(int, int, int, int)> dfs = [&](int u, int edgesUsed, int dist, int cost){

        if(visited[u]) return;
        if(dist >= bestDist) return;

        visited[u] = true;
        path.push_back(u);

        // If destination reached
        if(u == target){
            //Stops
            if(edgesUsed == K + 1){
                bestDist = dist;
                bestCost = cost;
                bestPath = path;
            }

            path.pop_back();
            visited[u] = false;
            return;

        }

        // Too many edges
        if(edgesUsed > K + 1){
            path.pop_back();
            visited[u] = false;
            return;
        }

        for(const Route& r : routes[u]){
            int v = getAirportIndex(r.neighbor);
            dfs(v, edgesUsed + 1, dist + r.distance, cost + r.cost);
        }

        path.pop_back();
        visited[u] = false;
    };

    dfs(src, 0, 0, 0);

    // Output

    if(bestDist == INT_MAX){
        cout << "Shortest route from " << origin << " to " << dest << " with " << K << " stops: None" << endl;
        return;
    }

    cout << "Shortest route from " << origin << " to " << dest << " with " << K << " stops: ";

    for(int i = 0; i < bestPath.size(); i++){
        cout << airports[bestPath[i]].getName();
        if(i != bestPath.size() - 1) cout << " -> ";
    }

    cout << ". The length is " << bestDist << ". The cost is " << bestCost << "." << endl;

}