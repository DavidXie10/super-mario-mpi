
#include "GameController.h"

/**
 * @brief main function of the program, it runs the game
 * @param argc how many arguments are on the command line
 * @param argv vector of strings with the arguments introduced
 * @return
 */
int main(int argc, char** argv)
{
    GameController game;
    return game.run(argc, argv);
}

