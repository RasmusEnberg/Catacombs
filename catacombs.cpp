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

    struct Item 
    {
        std::string name;
        std::string contextDescription;
    };
    std::vector<Item> items;

    std::string detailDescription;
    virtual void Describe()const {std::cout<< "Base room \n";}

    virtual ~Room() = default;
};
struct SecurityRoom : Room
{
    SecurityRoom()
    {
        name = "Security room";
        items.push_back({"Flashlight","a flashlight lying on the floor"});
            detailDescription = "one of the monitors shows there is "
                                "something patrolling the dark room";
        
    }
     void Describe() const override
    {std:: cout<< "You enter the securityroom after using the card, it's a small room with"
                  " monitors of cameras overseeing the museum";}
};
struct ToiletRoom : Room
{
    ToiletRoom()
    {
        name = "Toilet";
        items.push_back({"Security card", "a securitycard lying next to the sink."});

        detailDescription = "At the end of the counter there is a securitycard.";
    }
     void Describe() const override
    {std:: cout<< "You push open the heavy door to the museum toilets. "
                 "A wave of damp air and cleaning chemical stench greets you.";}
};
struct DinoRoom : Room
{
    DinoRoom()
    {
        name = "Dinosaur Exhibit";
        items.push_back({"Fossil bone", "a fossile bone lying on the floor"});
         detailDescription = "Dust covers the ancient fossils. The T-Rex skeleton towers overhead,\n "
                             "you can see another room to the north";
    }
    void Describe() const override
    {std:: cout<< "You enter the dinosaur exhibit. A massive T-Rex skeleton looms above!";}
};
struct SpaceRoom : Room
{
    SpaceRoom()
    {
        name ="Space Exhibit";
        
        detailDescription =
            "The room is bathed in soft, bluish light. Above, a spinning mobile of planets "
            "To your left, an astronaut suit stands behind glass. You can see there are 2 more rooms connected.";
    }
    void Describe() const override
    {std:: cout<< "You enter the space exhibit. Planets and stars spin in the dark.";}
};
struct EntranceRoom : Room
{
    EntranceRoom()
    {
        name = "Entrance room";
        detailDescription =
    "Tall banners hang from the ceiling, welcoming guests to the museum's newest exhibits.\n "
    "A brochure stand sits near the door, one leaflet half-folded, pointing toward "
    "the stairs to the ice age exhibit.";
    }
    void Describe() const override
    {std:: cout<< "You stand in the grand entrance hall.\n";}
};
struct IceageRoom : Room
{
    IceageRoom()
    {
        name = "Ice Age Exhibit";
        items.push_back({"Spear","a spear stuck to the side of a woolly mammoth"});
        detailDescription =
    "Cold mist fills the room, condensing on the glass that separates you "
    "from the towering figure of a woolly mammoth.";
    }
    void Describe() const override
    {std:: cout<< "Cold air greets you as you enter the Ice Age exhibit.";}
};
struct ClosedRoom : Room
{
    ClosedRoom()
    {
        name = "Closed of exhibit";
        detailDescription =
    "You can see a key attached to the collar of the dog, maby it fits the entrance door";

    }
    void Describe() const override
    {std:: cout<< "You can see there is a dog looking agressive patrolling forth and back lounging just outside your reach.\n";}
};

struct Game
{
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<std::string> inventory;
    std::vector<std::map<std::string, int>> exits;
    std::map<int, std::string> lockedRooms;
    bool dogPresent = true;     // Dog starts in the Closed Exhibit
    bool dogFriendly = false;   // Becomes true if player uses bone
    int currentRoom=0;
    bool flashlightOn = false;
    bool running = true;

    Game()
    {
        rooms.push_back(std::make_unique<EntranceRoom>()); // 0
        rooms.push_back(std::make_unique<DinoRoom>()); // 1
        rooms.push_back(std::make_unique<SpaceRoom>()); // 2
        rooms.push_back(std::make_unique<IceageRoom>()); // 3
        rooms.push_back(std::make_unique<ClosedRoom>()); // 4
        rooms.push_back(std::make_unique<SecurityRoom>()); // 5
        rooms.push_back(std::make_unique<ToiletRoom>()); // 6


        // Define exits for each room (like a map)
    exits.resize(rooms.size());

    // Entrance connects east to Dinosaur Room
    exits[0]["east"] = 1;
    exits[0]["west"] = 2;
    exits[0]["up"] = 3;



    // Dino connects west to Entrance
    exits[1]["west"] = 0;
    exits[1]["north"] = 6;
    

    // Space connects east to Entrance
    exits[2]["east"]  = 0;
    exits[2]["west"] = 4;
    exits[2]["north"] = 5;

    // Ice Age connects down back to entrance
    exits[3]["down"] = 0;
    

    // Closed Room connects east back to space room
    exits[4]["east"] = 2;

    // Security Room connects south to space room
    exits[5]["south"] = 2;

    // Toilets connects south to dino room
    exits[6]["south"] = 1;

    // Security Room needs security card
    lockedRooms[5] = "security card"; 



    }

        void pickUpItem(const std::string& itemName)
    {
            Room& room = *rooms[currentRoom];
        if (room.items.empty()) {
            std::cout << "There's nothing to pick up here.\n";
            return;
        }

        for (auto it = room.items.begin(); it != room.items.end(); ++it)
        {
            std::string roomItem = toLower(it->name);
            if (roomItem.find(itemName) != std::string::npos) {
                inventory.push_back(it->name);
                std::cout << "You picked up the " << it->name << "!\n";
                room.items.erase(it);
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
            std::cout << "\nYou see ";

            for (size_t i = 0; i < room.items.size(); ++i) {
                const auto& item = room.items[i];
                if (i > 0 && i == room.items.size() - 1)
                    std::cout << " and ";
                else if (i > 0)
                    std::cout << ", ";

                std::cout << item.contextDescription;
            }

            std::cout << ".\n";
        }
    }
        void Look()
    {
        Room& room = *rooms[currentRoom];
        if (!room.detailDescription.empty())
            std::cout << room.detailDescription << "\n";
        else
            std::cout << "There's nothing new to see.\n";

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
            int nextRoom = roomExits[direction];

            // === Flashlight restriction for Closed Exhibit (room 4) ===
            if (nextRoom == 4 && !flashlightOn)
            {
                std::cout << "It's pitch dark inside. You can barely see a thing, but you can hear something growling inside.\n";
                std::cout << "Maybe if you had a light source...\n";
                return; // stop here, don't run the locked-room code
            }
            else if (nextRoom == 4 && flashlightOn)
            {
                std::cout << "You shine your flashlight into the dark exhibit.\n";
            }

            // === Locked rooms (skip flashlight room) ===
            if (lockedRooms.count(nextRoom) && nextRoom != 4)
            {
                std::string requiredItem = lockedRooms[nextRoom];
                bool hasKey = false;

                for (const auto& item : inventory)
                {
                    if (toLower(item) == toLower(requiredItem))
                    {
                        hasKey = true;
                        break;
                    }
                }

                if (!hasKey)
                {
                    std::cout << "The door is locked. A keycard reader blinks red.\n";
                    std::cout << "You'll need a " << requiredItem << " to enter.\n";
                    return;
                }
                else
                {
                    std::cout << "You swipe the " << requiredItem
                            << ". The door unlocks with a beep.\n";
                    lockedRooms.erase(nextRoom);
                }
            }

            // === Normal movement ===
            currentRoom = nextRoom;
            std::cout << "\nYou walk " << direction << "...\n";
            DescribeCurrentRoom();
            // === Check for museum exit (Entrance Room) ===
            if (rooms[currentRoom]->name == "Entrance room")
            {
                bool hasKey = false;

                for (const auto& item : inventory)
                {
                    if (toLower(item) == "key")
                    {
                        hasKey = true;
                        break;
                    }
                }

                if (hasKey)
                {
                    std::cout << "\nYou approach the museum's front door and try the key...\n";
                    std::cout << "It fits perfectly. With a soft click, the lock releases.\n";
                    std::cout << "The heavy doors swing open, and a gust of cold night air hits your face.\n";
                    std::cout << "\n Congratulations! You've escaped the museum!\n";
                    running = false;
                }
                else
                {
                    std::cout << "The front door is locked tight. You'll need a key to get out.\n";
                }
            }
        }
        else
        {
            std::cout << "You can't go " << direction << " from here.\n";
        }
    }


    void parseInput(std::string input) 
    {
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
// #region Attack
        // === Attack ===
        if (verb == "attack") {
            if (tokens.size() >= 2) {
                std::string target;
                for (size_t i = 1; i < tokens.size(); ++i) {
                    if (i > 1) target += " ";
                    target += toLower(tokens[i]);
                }

                // --- Attack the dog in the Closed Exhibit ---
                if (target == "dog" || target == "the dog") {
                    if (currentRoom == 4 && dogPresent) {
                        bool hasSpear = false;
                        for (const auto& i : inventory) {
                            if (toLower(i) == "spear") {
                                hasSpear = true;
                                break;
                            }
                        }

                        if (!hasSpear) {
                            std::cout << "You lunge forward barehanded, but the dog snarls and leaps back.\n";
                            std::cout << "Without a weapon, that's not a good idea.\n";
                            return;
                        }

                        // Attack succeeds with spear
                        dogPresent = false;
                        dogFriendly = false;
                        std::cout << "You grip the spear tightly and thrust as the dog charges.\n";
                        std::cout << "With a pained yelp, the creature collapses and falls still.\n";
                        std::cout << "You catch your breath, realizing how close that was...\n";
                        std::cout << "Something glints on its collar — a  key.\n";

                        rooms[currentRoom]->items.push_back(
                            {"key", "a key lying near the fallen dog"}
                        );
                        return;
                    }
                    else if (currentRoom == 4 && !dogPresent) {
                        std::cout << "There's no dog left to attack.\n";
                        return;
                    }
                    else {
                        std::cout << "You swing your weapon wildly, but there's nothing to attack here.\n";
                        return;
                    }
                }

                // --- Default attack for other things ---
                std::cout << "You swing at the " << target << ", but nothing happens.\n";
                return;
            } else {
                std::cout << "Try: attack <target>\n";
                return;
            }
        }
 // #endregion Attack

        // === Use ===
    if (verb == "use") {
        //  Handle simple command like "use flashlight"
        if (tokens.size() == 2) {
            std::string item = toLower(tokens[1]);

            if (item == "flashlight") {
                bool hasFlashlight = false;

                // Check inventory for flashlight
                for (const auto& i : inventory) {
                    if (toLower(i) == "flashlight") {
                        hasFlashlight = true;
                        break;
                    }
                }

                if (!hasFlashlight) {
                    std::cout << "You don't have a flashlight to use.\n";
                    return;
                }

                // Toggle flashlight
                if (!flashlightOn) {
                    flashlightOn = true;
                    std::cout << "You switch on the flashlight. The beam cuts through the darkness.\n";
                } else {
                    flashlightOn = false;
                    std::cout << "You turn off the flashlight. Darkness closes in around you.\n";
                }
                return;
            }

                                    // --- Bone (dog interaction) ---
            if (item.find("bone") != std::string::npos)
            {
                bool hasBone = false;
                for (const auto& i : inventory)
                {
                    if (toLower(i) == "fossil bone" || toLower(i) == "bone") {
                        hasBone = true;
                        break;
                    }
                }

                if (!hasBone) {
                    std::cout << "You don't have a bone to use.\n";
                    return;
                }

                if (currentRoom == 4 && dogPresent)
                {
                    dogFriendly = true;
                    dogPresent = false;

                    std::cout << "You offer the fossil bone. The dog sniffs it cautiously, "
                                "then wags its tail and trots over to you.\n"
                                "Something shiny hangs from its collar — it's a key!\n";

                    rooms[currentRoom]->items.push_back(
                        {"key", "a small brass key lying on the floor near where the dog stood"}
                    );
                    return;
                }
                else if (currentRoom == 4 && !dogPresent)
                {
                    std::cout << "The dog is already gone.\n";
                    return;
                }
                else
                {
                    std::cout << "You wave the bone around, but nothing happens.\n";
                    return;
                }
            }


            std::cout << "You can't use that right now.\n";
            return;
        }

        //  Handle "use <item> on <target>" or "use <item> with <target>"
        auto it = std::find(tokens.begin(), tokens.end(), "on");
        if (it == tokens.end())
            it = std::find(tokens.begin(), tokens.end(), "with");

        std::string item, target;

        if (it != tokens.end() && it != tokens.begin() && (it + 1) != tokens.end()) {
            for (auto i = tokens.begin() + 1; i != it; ++i) {
                if (i != tokens.begin() + 1) item += " ";
                item += *i;
            }

            for (auto i = it + 1; i != tokens.end(); ++i) {
                if (i != it + 1) target += " ";
                target += *i;
            }

            item = toLower(item);
            target = toLower(target);


            // --- Flashlight special case ---
            if (item.find("flashlight") != std::string::npos) {
                bool hasFlashlight = false;
                for (const auto& i : inventory) {
                    if (toLower(i) == "flashlight") {
                        hasFlashlight = true;
                        break;
                    }
                }

                if (!hasFlashlight) {
                    std::cout << "You don't have a flashlight to use.\n";
                    return;
                }

                if (!flashlightOn) {
                    flashlightOn = true;
                    std::cout << "You switch on the flashlight and point it toward the " << target << ".\n";
                } else {
                    std::cout << "The flashlight is already on.\n";
                }
                return;
            }

            // Example: "use security card on door"
            if (item.find("security card") != std::string::npos && target.find("door") != std::string::npos) {
                std::cout << "You swipe the security card. The door unlocks with a soft beep.\n";
                return;
            }

            // Default action
            std::cout << "You try to use the " << item << " on the " << target
                    << ", but nothing happens.\n";
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

