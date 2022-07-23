#ifndef WORLDELEMENT_H
#define WORLDELEMENT_H

#include <iostream>
#include <vector>
#include "Mario.h"

/**
 * @brief The WorldElement class represents an abstract class of the world elements that Mario can interact with
 */
class WorldElement
{
public:
    /**
     * @brief Constructor of WorldElement class
     */
    WorldElement();

    /**
     * @brief Destructor of class WorldElement
     */
    virtual ~WorldElement();

    /**
     * @brief getActions returns a vector of strings consisting of the possible actions a worldElement can interact with Mario
     * @return the actions of the world element
     */
    virtual std::vector<std::string> getActions() const = 0;

    /**
     * @brief getActionProbability, according to the action selected, returns it's probability
     * @param action string selected
     * @return the probability of the action
     */
    virtual double getActionProbability(const std::string &action) const = 0;

    /**
     * @brief doAction interaction between Mario and the world element
     * @param mario process playing
     * @param action string selected
     * @param enemyArray either goombas or koopas to add an enemy
     */
    virtual void doAction(Mario& mario, std::string action, int* enemyArray) = 0;

    /**
     * @brief returnAction returns the message to be printed on terminal
     * @param action, the selected action
     * @return the message to be display on terminal
     */
    virtual std::string returnAction(const std::string action) = 0;
};

#endif // WORLDELEMENT_H
