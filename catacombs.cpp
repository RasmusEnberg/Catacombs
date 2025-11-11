#include <iostream>
#include <string>
#include <limits>
#include <ios>
#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip> // to make nice even spaces between text :)
#include <algorithm>
// använd setw(15) för få mellanrum, ändra 15 till vad som.



// split string by spaces
std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string word;
    while (iss >> word)
        tokens.push_back(word);
    return tokens;
}

// lowercase a string
std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}
// remove filler words like "the", "a", "an"
std::vector<std::string> removeFillerWords(const std::vector<std::string>& tokens) {
    std::vector<std::string> cleaned;
    for (const auto& t : tokens) {
        if (t != "the" && t != "a" && t != "an")
            cleaned.push_back(t);
    }
    return cleaned;
}

struct Room
{
    // 5 rum 
    std::string name;
    std::vector <std::string> items;
    std::string detailDescription;
    virtual void Describe()const {std::cout<< "Base room \n";}

    virtual ~Room() = default;
};
struct DinoRoom : Room
{
    DinoRoom()
    {
        name = "Dinosaur Exhibit";
        items.push_back("Fossil bone");
         detailDescription = "Dust covers the ancient fossils. The T-Rex skeleton towers overhead, "
                             "its shadow stretching across cracked tiles. Small footprints lead "
                             "toward a dark corner.";
    }
    void Describe() const override
    {std:: cout<< "You enter the dinosaur exhibit. A massive T-Rex skeleton looms above!\n ";}
};
struct SpaceRoom : Room
{
    SpaceRoom()
    {
        name ="Space Exhibit";
        items.push_back("Flashlight");
    }
    void Describe() const override
    {std:: cout<< "You enter the space exhibit. Planets and stars spin in the dark.\n";}
};
struct EntranceRoom : Room
{
    void Describe() const override
    {std:: cout<< "You stand in the grand entrance hall.\n";}
};
struct IceageRoom : Room
{
    IceageRoom()
    {
        name = "Ice Age Exhibit";
        items.push_back("Spear");
    }
    void Describe() const override
    {std:: cout<< "Cold air greets you as you enter the Ice Age exhibit.\n";}
};
struct ClosedRoom : Room
{
    void Describe() const override
    {std:: cout<< "You approach a roped-off section marked 'Expedition Exhibit — Opening Soon'. \n";}
};

struct Game
{
     std::vector<std::unique_ptr<Room>> rooms;
     std::vector<std::string> inventory;
     std::vector<std::map<std::string, int>> exits;
     int currentRoom=0;
    bool flashlight = false;
    bool running = true;

    Game()
    {
        rooms.push_back(std::make_unique<EntranceRoom>()); // 0
        rooms.push_back(std::make_unique<DinoRoom>()); // 1
        rooms.push_back(std::make_unique<SpaceRoom>()); // 2
        rooms.push_back(std::make_unique<IceageRoom>()); // 3
        rooms.push_back(std::make_unique<ClosedRoom>()); // 4


        // Define exits for each room (like a map)
    exits.resize(rooms.size());

    // Entrance connects north to Dinosaur Room
    exits[0]["east"] = 1;
    exits[0]["west"] = 2;
    exits[0]["up"] = 3;



    // Dino connects south (back) to Entrance, east to Space Room
    exits[1]["west"] = 0;
    //exits[1]["east"]  = 2;

    // Space connects east to Entrance,
    exits[2]["east"]  = 0;
    exits[2]["west"] = 4;

    // Ice Age connects south back to entrance
    exits[3]["down"] = 0;
    

    // Closed Room connects west back to space room
    exits[4]["east"] = 2;
    }

        void pickUpItem(const std::string& itemName)
    {
        Room& room = *rooms[currentRoom];
        if (room.items.empty()) {
            std::cout << "There's nothing to pick up here.\n";
            return;
        }

        // Try to find a matching item (case-insensitive, partial ok)
        for (auto it = room.items.begin(); it != room.items.end(); ++it)
        {
            std::string roomItem = toLower(*it);
            if (roomItem.find(itemName) != std::string::npos) {
                inventory.push_back(*it);
                room.items.erase(it);
                std::cout << "You picked up the " << *inventory.rbegin() << "!\n";
                return;
            }
        }

        std::cout << "You don't see a \"" << itemName << "\" here.\n";
    }
        void DescribeCurrentRoom()
    {
        Room& room = *rooms[currentRoom];
        std::cout << "\n--- " << room.name << " ---\n";
        room.Describe();

        if (!room.items.empty()) {
            std::cout << "You see:\n";
            for (const auto& i : room.items)
                std::cout << "  - " << i << "\n";
        }
    }
        void Look()
    {
        Room& room = *rooms[currentRoom];
        if (!room.detailDescription.empty())
            std::cout << room.detailDescription << "\n";
        else
            std::cout << "There's nothing new to see.\n";

        // Optionally remind the player of visible items
        if (!room.items.empty()) {
            std::cout << "\nYou notice:\n";
            for (const auto& i : room.items)
                std::cout << "  - " << i << "\n";
        }
    }

    
     void ShowInventory() const
    {
        std::cout << "\nYour inventory:\n";
        if (inventory.empty())
            std::cout << "(empty)\n";
        else
            for (auto& item : inventory)
                std::cout << " - " << item << "\n";
    }
        void Move(const std::string& direction)
    {
        auto& roomExits = exits[currentRoom];

        if (roomExits.find(direction) != roomExits.end())
        {
            currentRoom = roomExits[direction];
            std::cout << "\nYou walk " << direction << "...\n";
            std::cout << "--- " << rooms[currentRoom]->name << " ---\n";
            DescribeCurrentRoom();
        }
        else
        {
            std::cout << "You can’t go " << direction << " from here.\n";
        }
    }

    void parseInput(std::string input) {
        auto tokens = removeFillerWords(split(toLower(input)));
        if (tokens.empty()) return;

        std::string verb = tokens[0];

        if (verb == "look")
        {
            if (tokens.size() == 1 || (tokens.size() == 2 && tokens[1] == "around"))
            {
                Look();
            } 
            else 
            {
                std::cout << "You can 'look' or 'look around' to examine the room.\n";
            }
            return;
        }

                        // === Pick Up ===
        if (verb == "pick") {
            if (tokens.size() >= 3 && tokens[1] == "up") {
                // Build item name without trailing space
                std::string itemName;
                for (size_t i = 2; i < tokens.size(); ++i) {
                    if (i > 2) itemName += " ";
                    itemName += tokens[i];
                }

                // Clean up casing
                pickUpItem(toLower(itemName));
            } else {
                std::cout << "Try: pick up <item>\n";
            }
            return;
        }


        // === Movement ===
           if (verb == "n" || verb == "north" || verb == "s" || verb == "south" ||
            verb == "e" || verb == "east" || verb == "w" || verb == "west"||
            verb == "up"|| verb == "down" || verb == "d" || verb == "u") {

            // Normalize short directions
            std::string dir = verb;
            if (verb == "n") dir = "north";
            if (verb == "s") dir = "south";
            if (verb == "e") dir = "east";
            if (verb == "w") dir = "west";
            if (verb == "u") dir = "up";
            if (verb == "d") dir = "down";

            Move(dir);
            return;
        }



        // === Inventory ===
        if (verb == "inventory" || verb == "i") {
            ShowInventory();
            return;
        }

        // === Open ===
        if (verb == "open") {
            if (tokens.size() >= 2) {
                std::string thing;
                for (size_t i = 1; i < tokens.size(); ++i)
                    thing += tokens[i] + " ";
                std::cout << "→ Opening [" << thing << "]\n";
            } else {
                std::cout << "Try: open <thing>\n";
            }
            return;
        }

        // === Attack ===
        if (verb == "attack") {
            if (tokens.size() >= 2) {
                std::string target;
                for (size_t i = 1; i < tokens.size(); ++i)
                    target += tokens[i] + " ";
                std::cout << "→ Attacking [" << target << "]\n";
            } else {
                std::cout << "Try: attack <target>\n";
            }
            return;
        }

        // === Use ===
        if (verb == "use") {
            auto it = std::find(tokens.begin(), tokens.end(), "on");
            if (it == tokens.end())
                it = std::find(tokens.begin(), tokens.end(), "with");

            if (it != tokens.end() && it != tokens.begin() && (it + 1) != tokens.end()) {
                std::string item, target;

                for (auto i = tokens.begin() + 1; i != it; ++i)
                    item += *i + " ";

                for (auto i = it + 1; i != tokens.end(); ++i)
                    target += *i + " ";

                std::cout << "→ Using [" << item << "] on [" << target << "]\n";
            } else {
                std::cout << "Try: use <item> on <target>\n";
            }
            return;
        }

        // === Quit ===
        if (verb == "quit" || verb == "exit") {
            running = false;
            return;
        }

        // === Fallback ===
        std::cout << "I don't understand that. Try: use <item> on <target>\n";
    }
      void Play()
    {
    
        //bool running = true;


        std::cout << "Welcome to the Museum Adventure!\n";
        DescribeCurrentRoom();

        while (running)
        {
                std::cout << "\n> ";
            std::string input;
            std::getline(std::cin, input);
            parseInput(input);
        }
        std::cout << "Thanks for visiting the museum!\n";
        
    };

  
};


int main ()
{
    Game g;
    g.Play();
    
}

