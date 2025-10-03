#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <set>

using namespace std;

// Data structure to hold game input and following output
struct gameIO {
    string input;
    vector<string> outputs;
};

// current state of the game
struct gameState{
    string location;                        // current location in the game
    set<string> inventory;                  // Set inventory
    string action;                          // last action taken to get to the state
    string description;                     // game description at this state
};

vector<gameIO> parseGameIO(istream& in);
bool isNovel(gameState state, vector<gameState>& known_states);


