#ifndef WORLD_H
#define WORLD_H

#include "WorldElement.h"
#include <vector>
#include <queue>

/**
 * @brief The World class, the world of Mario
 */
class World
{
private:
    std::vector<std::queue<WorldElement*>> myWorld;     // vector of queues
    int bandera;                                        // Finish line
    int position;                                       // actual slot of the world

public:

    /**
     * @brief World class constructor
     */
    World();

    /**
     * @brief World class destructor
     */
    ~World();

    /**
     * @brief initialize the world from the file given
     */
    void initialize(std::string filename);

    /**
     * @brief getNext returns a queue to the next slot to be processed
     * @return queue of worldElements
     */
    std::queue<WorldElement*> getNext(std::string filename);

    /**
     * @brief getPosition returns the actual position in the World in which Mario is
     * @return position index
     */
    inline int getPosition(){return this->position;}

    /**
     * @brief createElement is a factory method that creates an element of the type indicated by the char element:
     * C: Coin
     * G: Goomba
     * H: Hole
     * K: KoopaTroopa
     * @param element
     * @return a worldElement pointer to dynamic library
     */
    static WorldElement* createElement(const char element);

    /**
     * @brief addEnemy adds the enemy to the world +10 positions from position
     * @param id char of enemy to add
     */
    void addEnemy(const char id);

private:
    /**
     * @brief destroyWorld destroys all the elements allocated in dynamic memory
     */
    void destroyWorld();
};

#endif // WORLD_H
