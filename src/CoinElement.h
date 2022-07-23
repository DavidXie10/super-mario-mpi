#ifndef COINELEMENT_H
#define COINELEMENT_H

#include "WorldElement.h"

/**
 * @brief The CoinElement class represents a floating block with an interrogation mark, when Mario encounters this object, then it may not jump or jump and hit the block
 */
class CoinElement : public WorldElement
{
public:
    /**
     * @brief Constructor of CoinElement class
     */
    CoinElement();

    /**
     * @brief Destructor of class CoinElement
     */
    virtual ~CoinElement() override;

    /**
     * @brief getActions returns a vector of strings consisting of the possible actions a coinElement can interact with Mario
     * @return "NO_JUMP" and "JUMP_AND_HIT_BLOCK" inside a vector
     */
    virtual std::vector<std::string> getActions() const override;

    /**
     * @brief getActionProbability, according to the action selected, returns it's probability
     * @param action string selected
     * @return the probability of the action
     */
    virtual double getActionProbability(const std::string& action) const override;

    /**
     * @brief doAction interaction between Mario and the coin element
     * @param mario process playing
     * @param action string selected
     * @param enemyArray not used
     */
    virtual void doAction(Mario& mario, std::string action, int* enemyArray) override;

    /**
     * @brief returnAction returns the message to be printed on terminal
     * @param action, the selected action
     * @return the message to be display on terminal
     */
    virtual std::string returnAction(const std::string action) override;
};

#endif // COINELEMENT_H
