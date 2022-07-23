#ifndef KOOPATROOPAELEMENT_H
#define KOOPATROOPAELEMENT_H

#include "WorldElement.h"

/**
 * @brief The KoopaTroopaElement class represents an enemy, inherits from WorldElement, when Mario encounters this object, then it may not jump, jump and pass or jump and kill the enemy
 */
class KoopaTroopaElement : public WorldElement
{
public:
    /**
     * @brief Constructor of KoopaTroopaElement class
     */
    KoopaTroopaElement();

    /**
     * @brief Destructor of class KoopaTroopaElement
     */
    virtual ~KoopaTroopaElement() override;

    /**
     * @brief getActions returns a vector of strings consisting of the possible actions a KoopaElement can interact with Mario
     * @return "NO_JUMP", "JUMP_AND_PASS" and "JUMP_AND_KILL_ENEMY" inside a vector
     */
    virtual std::vector<std::string> getActions() const override;

    /**
     * @brief getActionProbability, according to the action selected, returns it's probability
     * @param action string selected
     * @return the probability of the action
     */
    virtual double getActionProbability(const std::string& action) const override;

    /**
     * @brief doAction interaction between Mario and the KoopaTroopaElement
     * @param mario process playing
     * @param action string selected
     * @param enemyArray koopas to add an enemy
     */
    virtual void doAction(Mario &mario, std::string action, int* enemyArray) override;

    /**
     * @brief returnAction returns the message to be printed on terminal
     * @param action, the selected action
     * @return the message to be display on terminal
     */
    virtual std::string returnAction(const std::string action) override;
};

#endif // KOOPATROOPAELEMENT_H
