#include "GoombaElement.h"

GoombaElement::GoombaElement()
{

}

GoombaElement::~GoombaElement()
{
}

std::vector<std::string> GoombaElement::getActions() const{
    static const std::vector<std::string> actions = {"NO_JUMP", "JUMP_AND_PASS", "JUMP_AND_KILL_ENEMY"};
    return actions;
}

double GoombaElement::getActionProbability(const std::string &action) const{
    if(action == "NO_JUMP")
        return 0.05;
    else if(action == "JUMP_AND_PASS")
        return 0.55;
    else if(action == "JUMP_AND_KILL_ENEMY")
        return 0.40;
    else
        return 0.0;
}

void GoombaElement::doAction(Mario &mario, std::string action, int* enemyArray){
    if(action == "NO_JUMP")
         mario.killMario();
    else if(action == "JUMP_AND_KILL_ENEMY")
         enemyArray[mario.getEnemyRank()]++;                             //SUMAR AL ARRAY DE GOOMBAS + ENEMIGO
}

std::string GoombaElement::returnAction(const std::string action){
    if(action == "NO_JUMP")
         return " didn't jump and was killed by a little goomba";
    else if(action == "JUMP_AND_KILL_ENEMY")
        return " jumped and killed a little goomba";
    else
        return " jumped and dodge a little goomba";
}


