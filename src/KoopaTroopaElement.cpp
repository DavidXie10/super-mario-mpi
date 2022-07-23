#include "KoopaTroopaElement.h"

KoopaTroopaElement::KoopaTroopaElement()
{   
}

KoopaTroopaElement::~KoopaTroopaElement()
{
}

std::vector<std::string> KoopaTroopaElement::getActions() const{
    static const std::vector<std::string> actions = {"NO_JUMP", "JUMP_AND_PASS", "JUMP_AND_KILL_ENEMY"};
    return actions;
}

double KoopaTroopaElement::getActionProbability(const std::string &action) const{
    if(action == "NO_JUMP")
        return 0.10;
    else if(action == "JUMP_AND_PASS")
        return 0.53;
    else if(action == "JUMP_AND_KILL_ENEMY")
        return 0.37;
    else
        return 0.0;
}


void KoopaTroopaElement::doAction(Mario &mario, std::string action, int* enemyArray){
    if(action == "NO_JUMP")
         mario.killMario();
    else if(action == "JUMP_AND_KILL_ENEMY")   // MPI
        enemyArray[mario.getEnemyRank()]++;    //Array del enemigo
}

std::string KoopaTroopaElement::returnAction(const std::string action){
    if(action == "NO_JUMP")
        return " didn't jump and was killed by a Koopa troopa";
    else if(action == "JUMP_AND_KILL_ENEMY")   // MPI
        return " jumped and killed a Koopa troopa";
    else
        return " jumped and dodge a Koopa troopa";
}

