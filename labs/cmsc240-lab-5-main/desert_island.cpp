/*
Desert Island Adventure Game
Author: Inigo García-Velasco
Date: 09/26/2025
Class: CMSC 240 - Software Systems Development
Section: 01
Description: A text-based adventure game where the player is stranded on a desert island and must gather resources to build a raft and escape.
*/


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool has_item(string item, vector<string>& inventory) {
    /* Returns true if the player has the specified item in their inventory
        - item : item to check for
        - inventory : vector of items in player's inventory
    */
    return find(inventory.begin(), inventory.end(), item) != inventory.end();
}

void add_item(string item, vector<string>& inventory) {
    /* Adds the specified item to the player's inventory
        - item : item to add
        - inventory : vector of items in player's inventory
    */
    if (!has_item(item, inventory)) {
        inventory.push_back(item);
        cout << item << " added to inventory. \n" << endl;
    } else {
        cout << "[INVALID COMMAND] You already have " << item << " in your inventory. \n" << endl;
    }
}

void show_inventory(vector<string>& inventory) {
    /* Displays the player's current inventory
        - inventory : vector of items in player's inventory
    */
    cout << "Inventory: [";
    if (inventory.empty()) {
        cout << "Empty]" << endl;
    } else {
        for (const string& item : inventory) {
            cout << item << ", ";
        }
        cout << "]" << endl;
    }
}

void describe_location(string& location, vector<string>& inventory) {
    /* Describes the player's current location
        - location : current location of the player
    */
    if (location == "beach") {
        cout << "[LOCATION: " << location << "]" << endl;
        cout << "You are currently at the " << location << ". The waves crash gently on the shore, and a warm breeze carries the scent of salt and palms.\n" << endl;
        if (find(inventory.begin(), inventory.end(), std::string("palm leaf")) == inventory.end()) {
            cout << "You notice a large *palm leaf* lying nearby. It might be useful.\n" << endl;
        }
    } else if (location == "jungle") {
        cout << "[LOCATION: " << location << "]" << endl;
        cout << "You are currently at the " << location << ". The dense foliage is alive with the sounds of exotic birds and insects.\n" << endl;
        if (find(inventory.begin(), inventory.end(), std::string("wood")) == inventory.end()) {
            cout << "You notice some sturdy *wood* lying on the ground. It might be useful.\n" << endl;
        }
    } else if (location == "cave") {
        cout << "[LOCATION: " << location << "]" << endl;
        cout << "You are currently at the " << location << ". The air is cool and damp, and you can hear the distant sound of dripping water.\n" << endl;
        if (find(inventory.begin(), inventory.end(), std::string("rope")) == inventory.end()) {
            cout << "You notice a coil of *rope* hanging from the cave wall. It might be useful.\n" << endl;
        }
    } else if (location == "volcano") {
        cout << "[LOCATION: " << location << "]" << endl;
        cout << "You are currently at the " << location << ". The ground trembles slightly beneath your feet.\n" << endl;
        if (find(inventory.begin(), inventory.end(), std::string("stone")) == inventory.end()) {
            cout << "You notice some sharp *stone* scattered around. It might be useful.\n" << endl;
        }
    }
}

void show_map(const string& location) {
    /* Displays a simple map of the island with current location marked */
    cout << "-- MAP --" << endl;

    cout << (location == "beach"  ? "-> [Beach] (*)"  : "   [Beach]")  << endl;
    cout << "   |" << endl;
    cout << (location == "jungle" ? "-> [Jungle] (*)" : "   [Jungle]") << endl;
    cout << "   |" << endl;
    cout << "   " << (location == "cave"  ? "-> [Cave] (*)"   : "   [Cave]")   << endl;
    cout << "   |" << endl;
    cout << (location == "volcano" ? "-> [Volcano] (*)": "   [Volcano]")<< endl;
}

bool valid_cmd(string user_cmd, vector<string>& avail_cmds) {
    /* Returns true if the user_cmd is a valid possible command to use in the game
        - user_cmd : command input by user
        - avail_cmds : vector of available commands
    */
    for(string cmd : avail_cmds){
        if(user_cmd == cmd){
            return true;
        }
    }
    return false;
}


vector<string> get_avail_cmds(vector<string>& inventory, string& location) {
    /* Returns a vector of available commands based on the player's inventory and location
        - inventory : vector of items in player's inventory
        - location : current location of the player
        "get", "goto beach", "goto jungle", "goto cave", "goto volcano", "use", "show inventory", "show map", "help"
    */
    vector<string> avail_cmds = {"help", "show inventory", "show map"};

    if(location == "beach"){
        avail_cmds.push_back("goto jungle");
        avail_cmds.push_back("goto volcano");
        avail_cmds.push_back("get palm leaf");
        if (inventory.size() == 4){
            avail_cmds.push_back("get boat");
        }
        if (has_item("boat", inventory)){
            avail_cmds.push_back("use boat");
        }       
        if (has_item("palm leaf", inventory)){
            avail_cmds.push_back("use palm leaf");
        }
        if (has_item("rope", inventory)){
            avail_cmds.push_back("use rope");
        }
        if (has_item("stone", inventory)){
            avail_cmds.push_back("use stone");
        }
        if (has_item("wood", inventory)){
            avail_cmds.push_back("use wood");
        }

    } else if(location == "jungle"){
        avail_cmds.push_back("goto beach");
        avail_cmds.push_back("goto cave");
        avail_cmds.push_back("goto volcano");
        if (find(inventory.begin(), inventory.end(), std::string("wood")) == inventory.end()) {
            avail_cmds.push_back("get wood");
        }

    } else if(location == "cave"){
        avail_cmds.push_back("goto jungle");
        if (find(inventory.begin(), inventory.end(), std::string("rope")) == inventory.end()) {
            avail_cmds.push_back("get rope");
        }

    } else if(location == "volcano"){
        avail_cmds.push_back("goto beach");
        avail_cmds.push_back("goto jungle");
        if (find(inventory.begin(), inventory.end(), std::string("stone")) == inventory.end()) {
            avail_cmds.push_back("get stone");
        }
    }

    return avail_cmds;
}




int main() {
    vector<string> inventory;
    string location = "beach";
    vector<string> avail_cmds = get_avail_cmds(inventory, location);
    string user_cmd;
    cout << "Welcome to Desert Island!" << endl;
    cout << "[LOCATION: " << location << "]" << endl;
    cout << "You are currently at the " << location << ". The waves crash gently on the shore, and a warm breeze carries the scent of salt and palms.\n" << endl;
    if (find(inventory.begin(), inventory.end(), std::string("boat")) != inventory.end()){
        cout << "Congratulations! You have built a raft and can escape the island!" << endl;
    } else {
        cout << "Shipwrecked and alone, you must gather resources from the island to build a raft and escape.\n" << endl;
        if (find(inventory.begin(), inventory.end(), std::string("palm leaf")) == inventory.end()) {
            cout << "You notice a large *palm leaf* lying nearby. It might be useful.\n" << endl;
        }
    }
    cout << "What do you want to do? (type 'help' for a list of commands): " << endl;
    getline(cin, user_cmd);
    
    while (true) {
        avail_cmds = get_avail_cmds(inventory, location);
        if (!valid_cmd(user_cmd, avail_cmds)) {
            cout << "[INVALID COMMAND] Type 'help' for a list of commands.\n" << endl;
        } else if (user_cmd == "help") {
            cout << "Available commands:" << endl;
            for (const string& cmd : avail_cmds) {
                cout << "- " << cmd << endl;
            }
        } else if (user_cmd == "show inventory") {
            show_inventory(inventory);
        } else if (user_cmd == "show map") {
            show_map(location);
        } else if (user_cmd.rfind("goto", 0) == 0) {
            string new_location = user_cmd.substr(5);
            location = new_location;
            describe_location(location, inventory);
        } else if (user_cmd.rfind("get", 0) == 0) {
            string item = user_cmd.substr(4);
            add_item(item, inventory);
        } else if (user_cmd.rfind("use", 0) == 0) {
            string item = user_cmd.substr(4);
            if (has_item(item, inventory)) {
                if (item == "palm leaf") {
                    cout << "You use the palm leaf to create a makeshift sail for your raft.\n" << endl;
                } else if (item == "rope") {
                    cout << "You use the rope to secure parts of your raft together.\n" << endl;
                } else if (item == "stone") {
                    cout << "You use the stone to sharpen pieces of wood, making stronger parts for your raft.\n" << endl;
                } else if (item == "wood") {
                    cout << "You use the wood to build the frame of your raft.\n" << endl;
                } else if (item == "boat") {
                    cout << "Congratulations! You have successfully escaped the island!\n" << endl;
                    break;
                }
            } else {
                cout << "[INVALID COMMAND] You don't have " << item << " in your inventory. \n" << endl;
            }
        }
        cout << "What do you want to do next? (type 'help' for a list of commands)" << endl;
        getline(cin, user_cmd);
    }
    return 0;
}