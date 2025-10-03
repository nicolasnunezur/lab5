#include "ai_utils.h"

using namespace std;


vector<gameIO> parseGameIO(istream& in) {
    /*
        Parses the game output and returns a vector of gameIO structs.
        Each struct contains all game output lines and input commands for a single game session.
    */
    vector<gameIO> game_chunks;

    string line;
    string cur_input = "[GAME START]";          // Current input command
    vector<string> cur_output;                  // Current output lines after using the command
    gameIO cur_chunk;                           // Current gameIO chunk

    while (getline(in, line)) {
        if (line.rfind("%% ", 0) == 0) { // Line starts with "%% "
            // If we have a current chunk of output, save it before starting a new one
            if (!cur_input.empty() && !cur_output.empty()) {
                cur_chunk.input = cur_input;
                cur_chunk.outputs = cur_output;
                game_chunks.push_back(cur_chunk);

                // reset the current chunk
                cur_chunk = gameIO(); 
                cur_output.clear();     // reset the output
            }
            cur_input = line.substr(3); // Remove "%% " prefix and store the command used
        } else {
            cur_output.push_back(line); // Add line to current output
        }
    }

    // Add the last chunk if it exists
    if (!cur_input.empty() && !cur_output.empty()) {
        cur_chunk.input = cur_input;
        cur_chunk.outputs = cur_output;
        game_chunks.push_back(cur_chunk);
    }
    return game_chunks;
}

bool isNovel(gameState state, vector<gameState>& known_states) {
    /*
        Checks if the given game state is novel compared to known states.
        A state is considered novel if its location or inventory differs from all known states.
    */
    for (const auto& known_state : known_states) {
        if (state.location == known_state.location && 
            state.inventory == known_state.inventory && 
            state.description == known_state.description && 
            state.action == known_state.action) {
            return false; // Not novel if all members match
        }
    }
    return true; // Novel if no matches found
}