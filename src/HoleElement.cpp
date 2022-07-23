#include "HoleElement.h"

HoleElement::HoleElement()
{
}

HoleElement::~HoleElement()
{
}

std::vector<std::string> HoleElement::getActions() const{
    static const std::vector<std::string> actions = {"NO_JUMP", "JUMP_AND_PASS"};
    return actions;
}

double HoleElement::getActionProbability(const std::string &action) const{
    if(action == "NO_JUMP")
        return 0.05;
    else if(action == "JUMP_AND_PASS")
        return 0.95;
    else
        return 0.0;
}

void HoleElement::doAction(Mario &mario, std::string action, int* enemyArray){
    (void) enemyArray;
    if(action != "JUMP_AND_PASS")
       mario.killMario();
}

std::string HoleElement::returnAction(const std::string action){
    if(action == "JUMP_AND_PASS")
        return " jumped over the hole";
    else
        return " didn't jumped, falls into the hole";
}
