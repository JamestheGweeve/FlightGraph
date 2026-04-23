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

void AirportMap::insertAirport(const string n, const string c){
    AirportNode a(n, c);
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