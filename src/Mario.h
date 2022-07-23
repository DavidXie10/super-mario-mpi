#ifndef MARIO_H
#define MARIO_H

#include <cstdio>
#include <iostream>

/**
 * @brief The Mario class is as the name suggests represents Mario. He knows his coins, his position in the world his number,
 * his number of recognition, the enemy he is attacking, the total amount of marios, if his alive and his attack strategy.
 */
class Mario
{
private:
    size_t coins;           // The amount of coins Mario has
    size_t position;        // His position in the world
    size_t myRank;          // His number that recognizes him
    size_t enemyRank;       // The enemy his attacking
    size_t numProcesses;    // The total amount of Marios
    int isAlive;            // If his either alive or dead. 1 is alive and 0 is dead
    const char RLMA[4] = {'R', 'L', 'M', 'A'}; //The different attack strategies R = Random, L = Less Coins, M = More Coins, A = Attacker
    char attackStrategy;    // His attack strategy

public:
    /**
     * @brief Creates Mario with 0 coins, first position in world, his rank, enemy rank is randomized at creation, total processes, very much alive,
     * and his attack strategy randomized between options
     * @param His number of recognition, that differentiates him from other Marios.
     */
    Mario(int rank);

    /**
     * @brief Returns the amount of coins he has
     * @return the size_t of his coins
     */
    inline size_t getCoins(){ return this->coins;}

    /**
     * @brief Returns his position in the world
     * @return size_t with his position
     */
    inline size_t getPosition() { return this->position;}

    /**
     * @brief Returns if he is alive or dead
     * @return  1 if he is alive, 0 if he is dead
     */
    inline int getIsAlive(){ return this->isAlive;}

    /**
     * @brief Returns the number that identifies him
     * @return a size_t with his rank
     */
    inline size_t getRank(){return this->myRank;}

    /**
     * @brief Returns the number that identifies the Mario he is attacking
     * @return a size_t with the enemy rank
     */
    inline size_t getEnemyRank(){return this->enemyRank;}

    /**
     * @brief Returns the strategy he is using to choose the enemy rank
     * @return a char with the strategy: (R)andom, (L)ess Coins, (M)ore Coins, (A)ttacker
     */
    inline char getAttackStrategy(){return this->attackStrategy;}

    /**
     * @brief Get the full name Attack Strategy this Mario is using
     * @return The full string of the attack he is using. The options are: "RANDOM", "LESS COINS", "MORE COINS", "ATTACKER"
     */
    std::string getStrategyString();

    /**
     * @brief Increments the amount of coins that Mario has
     */
    inline void incrementCoins(){ ++this->coins; }

    /**
     * @brief Sets the position in the world
     * @param The new position he will be at
     */
    inline void setPosition(size_t newPosition) { position = newPosition;}

    /**
     * @brief Kills Mario. Sets his isAlive = 0.
     */
    inline void killMario(){ isAlive = 0;}

    /**
     * @brief Sets the enemy he will be attacking.
     * @param The rank of the enemy
     */
    inline void setEnemyRank(size_t enemy){ this->enemyRank = enemy;}

    /**
     * @brief Sets the strategy he will be using.
     * @param Char of the strategy to be used
     */
    inline void setStrategy(const char strategy){ this->attackStrategy = strategy;}
};

#endif // MARIO_H
