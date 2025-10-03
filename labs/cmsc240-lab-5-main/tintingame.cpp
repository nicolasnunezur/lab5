/*
    Tintin in Egypt Adventure
    Author: Nicolas Nunez Rodriguez
    Date: 09/25/2025

    Description:
    Adventure game where Tintin must collect 3 items 
    (Key, Papyri Box, Fuel) with the help of his dog Snowy 
    from different locations (Pyramid, Museum, Secret Base, Aerodrome) in order to win.
*/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<pair<string,string>> items = {
    {"Pyramid", ""},
    {"Secret Base", ""},
    {"Aerodrome", "Fuel"}
};

// Prints all available commands
void show_help() {
    cout << "[get]" << endl;
    cout << "[goto]" << endl;
    cout << "[use]" << endl;
    cout << "[show inventory]" << endl;
    cout << "[show map]" << endl;
    cout << "[help]" << endl;
}

// Prints the map (current location mark with (*))
void show_map(const vector<string>& locations, const string& current_location) {
    cout << "-- MAP --" << endl;
    for (string loc : locations) {
        if (loc == current_location) {
            cout << " - " << loc << " (*)" << endl;
        } else {
            cout << " - " << loc << endl;
        }
    }
}

// Shows player inventory
void show_inventory(const vector<string>& inventory) {
    if (inventory.empty()) {
        cout << "[EMPTY]" << endl;
    } else {
        cout << "-- INVENTORY --" << endl;
        for (string item : inventory) {
            cout << "[" << item << "]" << endl;
        }
    }
}

// Move player to another location and display if there is an object in that location
bool goto_location(const string& destination, const vector<string>& locations, string& current_location, const vector<pair<string,string>>& items) {
    for (string loc : locations) {
        if (loc == destination) {
            current_location = loc;
            cout << "[LOCATION: " << current_location << "]" << endl;
            cout << "You arrive at the " << current_location << "." << endl;

            bool found_item = false;
            for (auto p : items) {
                if (p.first == current_location && p.second != "") {
                    cout << "You see a [" << p.second << "] here." << endl;
                    found_item = true;
                }
            }

            if (!found_item) {
                if (current_location == "Pyramid") {
                    cout << "The chamber seems empty. Maybe Snowy can help." << endl;
                } else if (current_location == "Secret Base") {
                    cout << "The base looks abandoned. Something might be hidden in this locked box." << endl;
                } else if (current_location == "Museum") {
                    cout << "Ancient artifacts surround you, but nothing stands out." << endl;
                } else if (current_location == "Aerodrome") {
                    cout << "The aerodrome is empty. But there is a plane waiting nearby." << endl;
                }
            }
            return true;
        }
    }
    return false;
}


// Get an item from the current location
bool get_item(const string& current_location, vector<string>& inventory, vector<pair<string,string>>& items, const string& item_name) {
    for (auto& p : items) {
        if (p.first == current_location && p.second == item_name) {
            // Add object to the inventory
            inventory.push_back(item_name);
            cout << "You picked up the " << item_name << "!" << endl;

            p.second = "";  
            return true;
        }
    }
    return false;
}

// Use an item if available in inventory
bool use_item(const string& current_location, vector<string>& inventory, const string& item_name, vector<pair<string,string>>& items) {
    bool in_inventory = false;
    for (string item : inventory) {
        if (item == item_name) {
            in_inventory = true;
        }
    }
    if (!in_inventory) {
        return false;
    }
    if (current_location == "Pyramid" && item_name == "Snowy") {
        cout << "Snowy sniffs around and discovers a hidden Key in the sand!" << endl;
        for (auto& p : items) {
            if (p.first == "Pyramid" && p.second == "") {
                p.second = "Key";
                cout << "You see a [Key] here." << endl; 
            }
        }
        return true;
    }
    else if (current_location == "Secret Base" && item_name == "Key") {
        cout << "You used the Key to open a locked chest. Inside is the Papyri Box!" << endl;
        for (auto& p : items) {
            if (p.first == "Secret Base" && p.second == "") {
                p.second = "Papyri Box";
                cout << "You see a [Papyri Box] inside the unlocked box." << endl; 

            }
        }
        return true;
    }
    else if (current_location == "Aerodrome" && item_name == "Fuel") {
        if (inventory.size() == 4) {
            cout << "You fuel the plane and escape with all treasures!" << endl;
            cout << "[YOU WIN!]" << endl;
            exit(0);
        } else {
            cout << "[INVALID COMMAND] You need all 4 items to escape" << endl;
        }
        return true;
    }

    return false;
}


int main() {
    // Game locations
    vector<string> locations = {"Pyramid", "Museum", "Secret Base", "Aerodrome"};

    // Player inventory (starts with Snowy)
    vector<string> inventory = {"Snowy"};

    // Starting location
    string current_location = "Museum";

    cout << "[LOCATION: " << current_location << "]" << endl;
    cout << "Welcome to Tintin in Egypt!" << endl;
    cout << "Type 'help' to see available commands." << endl;

    while (true) {
        string command;
        cout << "> ";
        getline(cin, command);
        if (command == "help") {
            show_help();

        } else if (command == "show map") {
            show_map(locations, current_location);
        } else if (command == "show inventory") {
            show_inventory(inventory);
        } else if (command.rfind("goto", 0) == 0){
            string dest = command.substr(5);
            if (!goto_location(dest, locations, current_location, items)) {
                cout << "[INVALID COMMAND]" << endl;
            }
        } else if (command.rfind("get ", 0) == 0) {
            string item_name = command.substr(4);
            if (!get_item(current_location, inventory, items, item_name)) {
                cout << "[INVALID COMMAND]" << endl;
            }
        } else if (command.rfind("use ", 0) == 0) {
            string item_name = command.substr(4);
            if (!use_item(current_location, inventory, item_name, items)) {
                cout << "[INVALID COMMAND]" << endl;
            }
        } else {
            cout << "[INVALID COMMAND]" << endl;
        }
    }

    return 0;
}
