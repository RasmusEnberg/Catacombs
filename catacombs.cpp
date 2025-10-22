#include <iostream>
#include <string>
#include <limits>
#include <ios>
#include <iomanip> // to make nice even spaces between text :)
// använd setw(15) för få mellanrum, ändra 15 till vad som.
class Game
{
    // Game loop?
};
class Room
{
    // 5 rum
    public:
    void Describe()const{std::cout<< "Base room \n";}
};
class Item
{
    // 5 items , vector förodligen bättre
    // key,vapen,fackla,karta
};
class Commands
{
    // behöver vector i main för spara alla commands, gör alla checkar här ifall det är fel stavat etc
    // Kanske bättre att bara ha en method?
};

int main ()
{
    Room test;
    test.Describe();
    std::cout <<"Hello world";
    return 0;
}