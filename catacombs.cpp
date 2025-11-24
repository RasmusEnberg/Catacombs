#include <iostream>
#include <string>
#include <limits>
#include <ios>
#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>

// split string by spaces
std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> words;
    std::istringstream iss(str);
    std::string word;
    while (iss >> word)
        words.push_back(word);
    return words;
}

int writeSpeed = 0;
// Prints the text slow for dramatic effect
void PrintSlow(const std::string& text, int delayMs = writeSpeed)
{
    for (char c : text)
    {
        std::cout << c << std::flush;                 // print one character
        std::this_thread::sleep_for(
            std::chrono::milliseconds(delayMs));      // wait a bit
    }
}

// lowercase a string
std::string toLower(std::string s) 
{
    for (auto& c : s) c = std::tolower(c);
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

    virtual ~Room() = default;
    virtual void Describe()const {std::cout<< "Base room \n";}

    virtual void PrintHint() const
    {
        std::cout << "You look around, but no particular ideas come to mind.\n";
    }

};
struct SecurityRoom : Room
{
    SecurityRoom()
    {
        name = "Security room";
        items.push_back({"Flashlight","\nthere is a flashlight lying on a table"});
            detailDescription = "one of the monitors shows there is "
                                "something patrolling the dark room";     
    }
     void Describe() const override
    {PrintSlow("You enter the security room after using the card, it's a small room with"
               " monitors overseeing the museum.",writeSpeed);}

    void PrintHint() const override
    {
        PrintSlow("You are in the security room.\n",writeSpeed);
        PrintSlow("- Check the floor, desks, and consoles.\n",writeSpeed);
        PrintSlow("- Light might be important somewhere else.\n",writeSpeed);
    }
};
struct ToiletRoom : Room
{
    ToiletRoom()
    {
        name = "Toilet";
        items.push_back({"Security card", "\nThere is a securitycard lying next to the sink."});

        detailDescription = "All the stalldoors are open, and the floor tiles are shining brigthly. Noting of interest in here.";
    }
    void Describe() const override
    {PrintSlow("You push open the heavy door to the museum toilets.\n "
               "A wave of damp air and cleaning chemical stench greets you."),writeSpeed;}

    void PrintHint() const override
    {
        PrintSlow("You are in the toilets.\n",writeSpeed);
        PrintSlow("- Maybe there's something forgotten here.\n",writeSpeed);
    }
};
struct DinoRoom : Room
{
    DinoRoom()
    {
        name = "Dinosaur Exhibit";
        items.push_back({"Fossil bone", "\nYou see a fossile bone lying on the floor next to the T-Rex"});
         detailDescription = "Dust covers the ancient fossils. The T-Rex skeleton towers overhead,\n"
                             "you can see another room to the north";
    }
    void Describe() const override
    {PrintSlow("You enter the dinosaur exhibit. A massive T-Rex skeleton looms above!",writeSpeed);}

    void PrintHint() const override
    {
        PrintSlow("You are in the dinosaur exhibit.\n",writeSpeed);
        PrintSlow("- Look carefully at the floor and around the skeletons.\n",writeSpeed);
        PrintSlow("- Pick up items if you see any.\n",writeSpeed);
    }
};
struct SpaceRoom : Room
{
    SpaceRoom()
    {
        name ="Space Exhibit";
        
        detailDescription =
            "The room is bathed in soft, bluish light. Above, a spinning mobile of planets. "
            "To your left, an astronaut suit stands behind glass. You can see there are 2 more rooms connected.";
    }
    void Describe() const override
    {PrintSlow("You enter the space exhibit. Planets and stars spin in the dark.",writeSpeed);}

    void PrintHint() const override
    {
        PrintSlow("You are in the space exhibit.\n",writeSpeed);
        PrintSlow("- It's dark and creepy in some directions.\n",writeSpeed);
        PrintSlow("- A card or something electronic might be needed for the door.\n",writeSpeed);
    }
};
struct EntranceRoom : Room
{
    EntranceRoom()
    {
        name = "Entrance room";
        detailDescription = "Tall banners hang from the ceiling, welcoming guests to the museum's newest exhibits.\n "
                             "A brochure stand sits near the door, one leaflet half-folded, pointing toward "
                             "the stairs to the ice age exhibit.";
            // Add a backpack item
        items.push_back({"Backpack", "You notice a small backpack lying near the information desk."}
        );
    }
    void Describe() const override
    {PrintSlow("You stand in the grand entrance hall. \n",writeSpeed);}
    
    void PrintHint() const override
    {
        PrintSlow("You are in the entrance hall.\n ",writeSpeed);
        PrintSlow("- Maybe check left and right first.\n",writeSpeed);
        PrintSlow("- You might also find something useful to carry items with.\n",writeSpeed);
    }
};
struct IceageRoom : Room
{
    IceageRoom()
    {
        name = "Ice Age Exhibit";
        items.push_back({"Spear","There is a old wooden spear stuck to the side of a woolly mammoth"});
        detailDescription =  "Behind the mammoth there are neanderthals hiding in bushes getting ready for the hunt that have started";
    }
    void Describe() const override
    {PrintSlow("Cold air greets you as you enter the Ice Age exhibit.",writeSpeed);}

    void PrintHint() const override
    {
        PrintSlow("You are in the ice age exhibit.\n",writeSpeed);
        PrintSlow("- There might be a weapon hidden here.\n",writeSpeed);
    }
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
    {PrintSlow("You can see there is a dog looking agressive patrolling forth and back lounging just outside your reach.\n",writeSpeed);}

        void PrintHint() const override
    {
        PrintSlow("You are in the closed exhibit.\n",writeSpeed);
        PrintSlow("- You see a dog. Attacking unprepared might be a bad idea.\n",writeSpeed);
        PrintSlow("- Dogs usually like bones…\n",writeSpeed);
        //std::cout << "You are in the closed exhibit.\n";
        //std::cout << "- You see a dog. Attacking unprepared might be a bad idea.\n";
        //std::cout << "- Dogs usually like bones…\n";
    }
};

// dog states
enum class DogState {   
    Hostile,
    Friendly,
    Dead
};
DogState dogState = DogState::Hostile;

// flashlight states
enum class FlashlightState {
    Off,
    On,
    NotOwned
};

struct Game
{
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<std::string> inventory;
    std::vector<std::map<std::string, int>> exits;
    std::map<int, std::string> lockedRooms;
    bool securityDoorUnlocked = false;
    int currentRoom=0;
    bool flashlightOn = false;
    bool running = true;
    FlashlightState flashlightState = FlashlightState::NotOwned;
    
      // Check if the player has an item by name (partial match allowed)
    bool HasItem(const std::string& name)
    {
        for (const auto& item : inventory)
        {
            if (toLower(item).find(toLower(name)) != std::string::npos)
            {
                return true;
            } 
        }
        return false;
    };

       // Help, Prints out helpfull information
    void Help()
    {
        // Finds current room and prints help info 
        rooms[currentRoom]->PrintHint();
    }

    void ShowIntro()
{

    PrintSlow("You visit the legendary Mystery Museum.\n", writeSpeed);
    PrintSlow("One moment it's crowded and noisy — the next, everything goes dark...\n\n", writeSpeed);
    PrintSlow("When you wake, the lights are dim, the air is cold, and the museum is silent.\n", writeSpeed);
    PrintSlow("Night has fallen. The crowds are gone. You're alone in the Mystery Museum.\n\n", writeSpeed);
    PrintSlow("...few seconds later, you gather your courage and look around.\n\n", writeSpeed);

    std::this_thread::sleep_for(std::chrono::seconds(3));
}
       // Remove an item from inventory (first match)
    void RemoveItem(const std::string& name)
    {
        for (auto it = inventory.begin(); it != inventory.end(); ++it)
            if (toLower(*it).find(toLower(name)) != std::string::npos)
            {
                inventory.erase(it);
                return;
            }
    }

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

    // Entrance connects east to Dinosaur Room, up to iceage room, west to space room.
    exits[0]["east"] = 1;
    exits[0]["west"] = 2;
    exits[0]["up"] = 3;



    // Dino connects west to Entrance and north to bathroom
    exits[1]["west"] = 0;
    exits[1]["north"] = 6;
    

    // Space connects east to Entrance, north to security room, west to closed/dark room.
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
    }

    void HandleAttack(const std::vector<std::string>& tokens)
    {
        // Build target after verb
        std::string target;
        for (size_t i = 1; i < tokens.size(); i++)
        {
            if (i > 1) target += " ";
            target += toLower(tokens[i]);
        }

        target = toLower(target);

        if (target.find("dog") == std::string::npos)
        {
            PrintSlow("Attack what?\n",writeSpeed);
            return;
        }
        // Dog not here
        if (currentRoom != 4)
        {
            PrintSlow("There's no dog here.\n",writeSpeed);
            return;
        }

        // Dog friendly
        if (dogState == DogState::Friendly)
        {
            PrintSlow("You raise your weapon… but the dog looks at you with trusting eyes.\n"
                      "You lower the weapon. You can't do it.\n",writeSpeed);
            return;
        }

        // Already dead
        if (dogState == DogState::Dead)
        {
            PrintSlow("There's nothing left to attack.\n",writeSpeed);
            return;
        }

        // Hostile but check weapon
        if (!HasItem("spear"))
        {
            PrintSlow("You lunge barehanded, but the dog snarls and leaps back.\n"
                      "Without a weapon, that's not a good idea.\n",writeSpeed);
            return;
        }

        // Success (dog dies)
        dogState = DogState::Dead;
        PrintSlow("You thrust the spear — the dog collapses.\n"
                  "With a pained yelp, the creature falls still.\n"
                  "Something glints on its collar — a key.\n",writeSpeed);

        rooms[currentRoom]->items.push_back({"key", "A key dropped from the dog."});
    }

    void pickUpItem(const std::string& rawItemName)
    {
        Room& room = *rooms[currentRoom];

        if (room.items.empty())
        {
            PrintSlow("There's nothing to pick up here.\n",writeSpeed);
            return;
        }

        std::string itemName = toLower(rawItemName);

        for (auto it = room.items.begin(); it != room.items.end(); ++it)
        {
            std::string roomItem = toLower(it->name);

            // checks if this room item match what the player typed
            if (roomItem.find(itemName) != std::string::npos)
            {

                
                bool pickingBackpack = (roomItem == "backpack"); // checks if the player is in the correct room for picking backpack
                bool hasBackpack    = HasItem("backpack");

                if (!pickingBackpack && !hasBackpack)
                {
                    // Count how many NON backpack items the player already carries
                    int carriedNonBackpack = 0;
                    for (const auto& inv : inventory)
                    {
                        if (toLower(inv) != "backpack")
                            ++carriedNonBackpack;
                    }

                    if (carriedNonBackpack >= 1) // checks how many items you have before having the backpack.
                    {
                        PrintSlow("Your hands are full. Without a backpack you can only carry one item.\n"
                                  "Maybe you should find something to carry items with.\n",writeSpeed);
                        return;
                    }
                }

                // Picking up the backpack
                inventory.push_back(it->name);
                std::string msg ="You picked up the " + it->name + "!\n";
                PrintSlow(msg,writeSpeed);

                // Flashlight state tracking 
                if (roomItem == "flashlight")
                {
                    flashlightState = FlashlightState::Off;
                    PrintSlow("You now have a flashlight. Try to use it.\n",writeSpeed);
                }

                room.items.erase(it);
                return;
            }
        }
        std::string msg = "You don't see a \"" + rawItemName + "\" here.\n";
        PrintSlow(msg,writeSpeed);
    }

    void DescribeCurrentRoom()
    {
        Room& room = *rooms[currentRoom];
        std::cout << "\n--- " << room.name << " ---\n";
        room.Describe();

        if (!room.items.empty())
        {        
            for (size_t i = 0; i < room.items.size(); ++i)
            {
                // print each item description with delay
                PrintSlow(room.items[i].contextDescription, writeSpeed);

                if (i < room.items.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << ".\n";
        }
    }

    void Look()
    {
        Room& room = *rooms[currentRoom];
        if (!room.detailDescription.empty())
        {
            std::string msg = room.detailDescription + "\n";          
            PrintSlow(msg,writeSpeed);
        }
        else
        {
            PrintSlow("There's nothing new to see.\n",writeSpeed);
        }
    }

    void ShowInventory() const
    {
        std::cout << "Inventory:\n";

        if (inventory.empty())
        {
            PrintSlow("You aren't carrying anything. \n",writeSpeed);
            return;
        }
        // Sorted copy vector 
        std::vector<std::string> sorted = inventory;
        std::sort(sorted.begin(), sorted.end());

        for (const auto& item : sorted)
        {
            std::cout << "  • " << item << "\n";
        }
    }

    void Move(const std::string& direction)
    {
        // Get exits from current room
        auto& roomExits = exits[currentRoom];

        // Check if movement direction exists
        if (roomExits.find(direction) == roomExits.end())
        {
            std::string msg= "You can't go " + direction + " from here.\n";
            PrintSlow(msg,writeSpeed);
            return;
        }

        int nextRoom = roomExits[direction];
        // Checks if you have the security key to enter security room
        if (currentRoom == 2 && nextRoom == 5 && !securityDoorUnlocked)
        {
            PrintSlow("The door is locked. A keypad blinks red.\n"
                      "You'll need a security card to enter.\n",writeSpeed);
            return;
        }

        // Flashlight requirement for Closed Exhibit
        const int dogRoom = 4;
        if (nextRoom == dogRoom && flashlightState != FlashlightState::On)
        {
            PrintSlow("It's pitch dark inside. You can barely see, but you hear low growling...\n Maybe if you had a light source...\n",writeSpeed);
            return;
        }

        // Locked rooms 
        if (lockedRooms.count(nextRoom) && nextRoom !=4 && nextRoom !=5)
        {
            std::string requiredItem = lockedRooms[nextRoom];

            std::string msg = "You swipe the " + requiredItem + ". The lock clicks open.\n";
            PrintSlow(msg,writeSpeed);
            lockedRooms.erase(nextRoom);
        }

        // Apply movement 
        currentRoom = nextRoom;
        std::cout << "\nYou walk " << direction << "...\n";

        DescribeCurrentRoom();

        //Exit game check 
        if (currentRoom == 0)
        {
            if (HasItem("key"))
            {
                PrintSlow("\nYou approach the museum's front door and try the key...\n"
                          "It fits perfectly. With a soft click, the lock releases.\n"
                          "Cold night air rushes in as the heavy doors swing open.\n"
                          "Congratulations — you've escaped the museum!\n",writeSpeed);
                running = false;
            }
            else
            {
                PrintSlow("The front door is locked tight. You still need a key.\n",writeSpeed);
            }
        }
    }
    bool SecurityCardUse(const std::string& verb,
                        std::string& itemWord,
                        std::string& target)
{
    // Check if this is about the security card
    if (itemWord.find("security") == std::string::npos &&
        itemWord.find("card")     == std::string::npos &&
        itemWord.find("keycard")  == std::string::npos)
    {
        return false; // not a card-related command
    }

    // Check that the player is trying to use it on the door / keypad,
    // or has no target but is standing in the correct room
    if (target.find("door")   == std::string::npos &&
        target.find("keypad") == std::string::npos &&
        !(target.empty() && currentRoom == 2))   // 2 = Space Exhibit
    {
        return false; // not our case
    }

    // If no target but in the correct room, assume "door"
    if (target.empty() && currentRoom == 2)
        target = "door";

    // Player must be standing at the correct door
    if (currentRoom != 2)
    {
        PrintSlow("There's nothing here to use the card on.\n",writeSpeed);
        return true; 
    }

    // Check if the door is already unlocked
    if (securityDoorUnlocked)
    {
        PrintSlow("The door is already unlocked.\n",writeSpeed);
        return true;
    }

    // Check if the player actually has the card
    if (!HasItem("security"))
    {
        PrintSlow("You pat your pockets... You don't seem to have a security card.\n",writeSpeed);
        return true;
    }

    // Unlock the door
    securityDoorUnlocked = true;
    PrintSlow("You swipe the security card. A green light flashes — the lock clicks open.\n",writeSpeed);
    return true;
}

    void parseInput(std::string input)
    {
        // These will be filled when we handle Use-like commands
        std::string itemWord;
        std::string target;

        auto tokens = removeFillerWords(split(toLower(input)));
        if (tokens.empty()) return;

        std::string verb = tokens[0];

        // Help
        if (verb == "help")
        {
            Help();
            return;
        }

        // New verbs for giving dog bone
        if (verb == "give" || verb == "offer")
        {
            verb = "use";
        }

        // Look
        if (verb == "look")
        {
            if (tokens.size() == 1 || (tokens.size() == 2 && tokens[1] == "around"))
            {
                Look();
            }
            else
            {
                PrintSlow("Try to look around.\n",writeSpeed);
            }
            return;
        }

        // Pick / Take item
        if (verb == "pick" || verb == "take" ||verb == "grab" ||verb == "get")
        {
            // Need at least a verb + something
            if (tokens.size() < 2)
            {
                PrintSlow("Try to pick up item or use item\n",writeSpeed);
                return;
            }

            // Where does the item name start?
            // pick bone   -> start at tokens[1]
            // pick up bone -> start at tokens[2]
            size_t startIndex = 1;
            if (tokens.size() >= 3 && tokens[1] == "up")
            {
                startIndex = 2;
            }

            // If we only got "pick up" with no item
            if (startIndex >= tokens.size())
            {
                PrintSlow("Try to pick up item or use item\n",writeSpeed);
                return;
            }

            // Build item name from the remaining words
            std::string itemName;
            for (size_t i = startIndex; i < tokens.size(); ++i)
            {
                if (i > startIndex) itemName += " ";
                itemName += tokens[i];
            }

            pickUpItem(toLower(itemName));
            return;
        }

        // Movement
        if (verb == "n" || verb == "north" ||
            verb == "s" || verb == "south" ||
            verb == "e" || verb == "east"  ||
            verb == "w" || verb == "west"  ||
            verb == "u" || verb == "up"    ||
            verb == "d" || verb == "down")
        {
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

        // Inventory
        if (verb == "inventory" || verb == "i")
        {
            ShowInventory();
            return;
        }

        // Attack (delegates to your attack handler)

        if (verb == "attack" || verb == "kill" ||
            verb == "stab"   || verb == "fight")
        {
            HandleAttack(tokens);
            return;
        }

        // Use / Swipe / Scan / Tap
        if (verb == "use" || verb == "swipe" ||
            verb == "scan" || verb == "tap")
        {
            if (tokens.size() < 2)
            {
                std::cout << "Use what?\n";
                return;
            }


            // Build itemWord = words after verb up to "on"/"with"
            size_t splitIndex = tokens.size();
            auto it = std::find(tokens.begin() + 1, tokens.end(), "on");
            if (it == tokens.end())
                it = std::find(tokens.begin() + 1, tokens.end(), "with");
            if (it != tokens.end())
                splitIndex = static_cast<size_t>(it - tokens.begin());

            itemWord.clear();
            for (size_t i = 1; i < splitIndex; ++i)
            {
                if (i > 1) itemWord += " ";
                itemWord += tokens[i];
            }

            // Build target = words after "on"/"with"
            target.clear();
            if (splitIndex < tokens.size())
            {
                for (size_t i = splitIndex + 1; i < tokens.size(); ++i)
                {
                    if (i > splitIndex + 1) target += " ";
                    target += tokens[i];
                }
            }

            // Security card handler
            if (SecurityCardUse(verb, itemWord, target))
                return; // card action handled


            if (verb != "use")
            {
                std::string msg = "You " + verb + " the " + itemWord + ", but nothing special happens.\n";
                PrintSlow(msg,writeSpeed);

                return;
            }

            // Use Flashlight
            if (itemWord.find("flashlight") != std::string::npos)
            {
                if (flashlightState == FlashlightState::NotOwned)
                {
                    PrintSlow("You don't have a flashlight.\n",writeSpeed);
                    return;
                }

                if (flashlightState == FlashlightState::Off)
                {
                    flashlightState = FlashlightState::On;
                    PrintSlow("You switch on the flashlight. The beam cuts through the darkness.\n",writeSpeed);
                }
                else if (flashlightState == FlashlightState::On)
                {
                    flashlightState = FlashlightState::Off;
                    PrintSlow("You turn off the flashlight. Darkness closes in.\n",writeSpeed);
                }
                return;
            }

            // Use bone, Dog interaction
            if (itemWord.find("bone") != std::string::npos &&
                (target.find("dog") != std::string::npos || target.empty()))
            {
                const int dogRoom = 4; // Closed exhibit
                if (currentRoom != dogRoom)
                {
                    PrintSlow("There's no dog here.\n",writeSpeed);
                    return;
                }

                if (!HasItem("bone"))
                {
                    PrintSlow("You don't have a bone.\n",writeSpeed);
                    return;
                }

                // Dog reactions based on state
                if (dogState == DogState::Dead)
                {
                    std::cout << "The dog is dead.\n";
                    return;
                }

                if (dogState == DogState::Friendly)
                {
                    std::cout << "The dog happily wags its tail, it already trusts you.\n";
                    return;
                }

                // Hostile -- Friendly transition
                dogState = DogState::Friendly;
                RemoveItem("bone");
                PrintSlow("You offer the fossil bone. The dog sniffs it cautiously,\n"
                          "then wags its tail and trots over to you.\n"
                          "Something shiny swings from its collar — a key drops to the floor.\n",writeSpeed);

                rooms[currentRoom]->items.push_back(
                    {"key", "a small key lying on the ground"});
                return;
            }

            // Fallback USE response
            std::string msg = "You try to use the " + itemWord + " on " + target + ", but nothing happens.\n";
            PrintSlow(msg,writeSpeed);
            return;
        }


        // Quit
        if (verb == "quit" || verb == "exit")
        {
            running = false;
            return;
        }

        // Unknown command
        PrintSlow("I don't understand that. Try proper commands(Use,Look,Grab)",writeSpeed);
    }
    void Play()
    {
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
    g.ShowIntro();
    g.Play();
    
}

