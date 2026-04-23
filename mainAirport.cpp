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
        int s;
        string last_word;
        
        if(out.size() >= 1){
            s = out.size()-1;
            last_word = out[s];
        }
        if(t[3] == '\"' && !last_word.empty()){
            if(last_word[last_word.size()-1] != '\"'){
                out[out.size()-1] = out[out.size()-1] + "," + t;
            }
            else{
                out.push_back(t);
            }
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
    if(!m.hasAirport(AirportNode(words[0], words[2]))){
        m.insertAirport(words[0], words[2]);
    }
    if(!m.hasAirport(AirportNode(words[1], words[3]))){
        m.insertAirport(words[1], words[3]);
    }
    
    m.insertRoute(AirportNode(words[0], words[2]), AirportNode(words[1], words[3]), stoi(words[4]), stoi(words[5]));
}