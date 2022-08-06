#include <iostream>

#include "Game.hpp"

int main(int argc, char* argv[])
{
    try
    {
        Game game;
        game.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
