
#include "CoinElement.h"

CoinElement::CoinElement()
{
}

CoinElement::~CoinElement()
{
}

std::vector<std::string> CoinElement::getActions() const{
    static const std::vector<std::string> actions = {"NO_JUMP", "JUMP_AND_HIT_BLOCK"};
    return actions;
}

double CoinElement::getActionProbability(const std::string &action) const{
    if(action == "NO_JUMP")
        return 0.5;
    // Mario jumps and hit the block
    else if(action == "JUMP_AND_HIT_BLOCK")
        return 0.5;
    else
        return 0.0;
}

void CoinElement::doAction(Mario &mario, std::string action, int* enemyArray){
   (void)enemyArray;
   if(action == "JUMP_AND_HIT_BLOCK")
        mario.incrementCoins();
}

std::string CoinElement::returnAction(const std::string action){
    if(action == "JUMP_AND_HIT_BLOCK")
         return " jumped and grabbed a coin!";
    else
        return " didn't grab a coin";
}
