#include "Airport.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <cctype>

vector<string> split(const string& s);
void addRow(vector<string> words, AirportMap& m);
void runFromFile(const char* file_name, AirportMap& m);

int main(int argc, char** argv){
    if (argc != 2) {
        std::cerr << "Error: incorrect number of arguments\n";
        return 1;
    }
    
    AirportMap m;
    try {
        runFromFile(argv[1], m);
        m.print();
    }
    catch(std::logic_error& ex) {
        std::cout << ex.what() << std::endl;
    }


    return 0;
}

vector<string> split(const string& s) {
    vector<string> out;
    regex del(",");
    // Create a regex_token_iterator to split the string
    sregex_token_iterator it(s.begin(), s.end(), del, -1);

    // End iterator for the 
    // regex_token_iterator
    sregex_token_iterator end;

    // Iterating through each token
    while (it != end) {
        string t = *it;
        if(t[0] == '\"'){
            t = t.substr(1);
            out.push_back(t);
        }
        else if(t[3] == '\"'){
            t = t.substr(0,t.size()-1);
            out.push_back(t);
        }
            
        else{
            out.push_back(*it);
        }
        ++it;
    }

    return out;
}


void runFromFile(const char* file_name, AirportMap& m) {
    ifstream in;

    // Open the file
    in.open(file_name);

    // Check if file exists
    if (in.fail()) {
        cerr << file_name << " could not be opened\n";
        throw logic_error("File could not be opened\n");
    }

    string line;
    vector<string> words;
    getline(in, line);
    while (getline(in, line)) {
        words = split(line);
        addRow(words, m);
    }

    // Close the file
    in.close();
}

void addRow(vector<string> words, AirportMap& m){
    if(!m.hasAirport(AirportNode(words[0], words[2], words[3]))){
        m.insertAirport(words[0], words[2], words[3]);
    }
    if(!m.hasAirport(AirportNode(words[1], words[4], words[5]))){
        m.insertAirport(words[1], words[4], words[5]);
    }
    
    m.insertRoute(AirportNode(words[0], words[2], words[3]), AirportNode(words[1], words[4], words[5]), stoi(words[6]), stoi(words[7]));
}