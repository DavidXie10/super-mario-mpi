#include "Mario.h"

Mario::Mario(int rank): coins(0) , position(1), myRank(rank), enemyRank(0), isAlive(1), attackStrategy(this->RLMA[rand() % 4])
{
}

std::string Mario::getStrategyString(){
    switch(this->attackStrategy){
    case 'R':
        return "RANDOM";
    case 'L':
        return "LESS COINS";
    case 'M':
        return "MORE COINS";
    case 'A':
        return "ATTACKER";
    }
    return "nullptr";
}

