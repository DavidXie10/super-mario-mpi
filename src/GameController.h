#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <iostream>
#include <queue>
#include "Mario.h"
#include "World.h"
#include "WorldElement.h"

/**
 * @brief The GameController class controls the game logic and runs the SuperMario 35 game with the number of processes indicated on the command line
 */
class GameController
{
private:
    int* mariosAlive;       // marios left playing array
    int* mariosCoins;       // every mario's coins amount ordered according to it's rank
    int* attacking;         // the rank's array of the Marios that each index process is attacking
    int* goombas;           // an array with the amount of goombas to be added to each mario process after an iteration of the worldElement
    int* koopaTroopas;      // an array with the amount of koopas to be added to each mario process after an iteration of the worldElement
    int numProcesses;       // the total amount of processes that run the program
    bool clusterRun;        // flag that indicate wether is runnign on cluster or not

public:
    /**
     * @brief Constructor of GameController class
     */
    GameController();

    /**
     * @brief Destructor of GameController class, destroys everything allocated on dynamic memory to avoid memory leaks
     */
    ~GameController();

    /**
     * @brief runs the game
     * @param argc how many arguments are on the command line
     * @param argv vector of strings with the arguments introduced
     * @return 0 if everything went just fine, otherwise it throws an exception
     */
    int run(int argc, char **argv);

private:
    /**
     * @brief initializes the private attributes of the class in dynamic memory
     */
    void initialize();

    /**
     * @brief checkBattleStatus checks if there is more than 1 player playing, if there is only one Mario left, then the game finishes
     * @return the amount of marios that are playing, 0 if there is only one more player
     */
    int checkBattleStatus();

    /**
     * @brief chooseEnemyRank, according to the strategy selected for each Mario, it chooses the enemy rank to attack, only if it is alive and it is not myself
     * @param myMario to set the enemyRank
     */
    void chooseEnemyRank(Mario &myMario);

    /**
     * @brief attackRandom selects a random index to attack according to the 'R' strategy
     * @param myMario
     * @return random index rank to attack
     */
    int attackRandom(Mario &myMario);

    /**
     * @brief leastCoins selects the player whose coins are the minimun among all processes
     * @param myMario
     * @return least coins mario's rank
     */
    int leastCoins(Mario &myMario) const;

    /**
     * @brief mostCoins selects the player whose coins are the maximun among all processes
     * @param myMario
     * @return most coins mario's rank
     */
    int mostCoins(Mario &myMario)const;

    /**
     * @brief attackingEnemy
     * @param myMario
     * @return
     */
    int attackingEnemy(Mario &myMario);

    /**
     * @brief synchronize gathers or reduce from all processes the values related to their coins count, if it is alive or not, the number of goombas and koopas send to other worlds and the enemy rank of each process, all these values are stored in their corresponding array ordered by index. Also, if the rank is 0 and the mario to see on terminal is dead, then ask for the user for another input mario from the list of mariosAlive. If it is running on a cluster, then it choses it randomly
     * @param myMario process
     * @param coins of each mario
     * @param alive value of each mario
     * @param goombasSendBuffer amount of goombas sent
     * @param koopasSendBuffer amount of koopas sent
     * @param enemyRank who I am attacking
     * @param marioToSee who to print on terminal
     */
    void synchronize(Mario &myMario, int &coins, int &alive, int* goombasSendBuffer, int* koopasSendBuffer, int &enemyRank, int &marioToSee);

    /**
     * @brief validateInputData checks if the arguments in the command line are permited, otherwise it prints the usage
     * @param rank, only the rank 0 will check it
     * @param argc how many arguments are on the command line
     * @param argv vector of strings with the arguments introduced
     * @param marioToSee on terminal and broadcasted to all, the first argument
     * @param strategy, the second argument
     */
    void validateInputData(const int rank, const int argc, char** argv, int& marioToSee, char &strategy);

    /**
     * @brief advancePosition for each element in the queue position of the world given, pops every element and interact with it until Mario is dead oe the world has no more elements
     * @param myQueue queue of the worldElement elements to interact with
     * @param myMario to interact with the elements
     * @param goombasSendBuffer if the element is a goomba, sends this array to the element action
     * @param koopasSendBuffer if the element is a koopa, sends this array to the element action
     * @param marioToSee to be printed on terminal
     * @param myWorld in which I am playing
     */
    void advancePosition(std::queue<WorldElement *> &myQueue, Mario& myMario, int* goombasSendBuffer, int* koopasSendBuffer, const size_t marioToSee, World& myWorld);

    /**
     * @brief printMario prints the message to be displayed on terminal
     * @param myWorld played
     * @param myMario process
     * @param stringAction what to be done with an element
     */
    void printMario(World& myWorld, Mario& myMario, std::string stringAction);

    /**
     * @brief searchWinner searches who is the last mario standing rank
     * @return rank of the last Mario alive
     */
    int searchWinner()const;

    /**
     * @brief isValid checks wether the letter inputed on the command line correspond to a strategy available
     * @param chosen the second argument of the command line
     * @return true if it is valid, false otherwise
     */
    inline bool isValid(const char chosen){
        return chosen == 'R' || chosen == 'L' || chosen == 'M' || chosen == 'A';
    }

    /**
     * @brief throwError throws an error to the try catch section when encountered
     * @param error number
     * @param strError string to be printed
     */
    void throwError(const int error, std::string strError)const;

    /**
     * @brief update updates the values after an iteration
     * @param coins
     * @param alive
     * @param enemyRank
     * @param myMario
     * @param goombasSendBuffer
     * @param koopasSendBuffer
     * @param marioToSee
     */
    void update(int& coins, int& alive, int& enemyRank, Mario& myMario, int* goombasSendBuffer, int* koopasSendBuffer, int& marioToSee);

    /**
     * @brief addEnemies into the world, both goombas and koopas
     * @param myWorld
     * @param myMario
     */
    void addEnemies(World &myWorld, Mario &myMario);

    /**
     * @brief printBoard prints a line of asteriks
     */
    inline void printBoard(){
        std::cout << "\n*********************************\n";
    }
};

#endif // GAMECONTROLLER_H
