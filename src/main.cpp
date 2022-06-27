#include <iostream>

#include "Game.hpp"

int main(int argc, char* argv[])
{
    try
    {
        auto game = std::make_unique<Game>();

        game->init();
        game->loadResource();

        while (game->isRunning())
        {
            game->handleEvents();
            game->draw();
        }

        game->unloadResource();
        game.reset();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
