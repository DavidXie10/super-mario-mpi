
#include "World.h"
#include <fstream>

World::World():position(0)
{    
}

World::~World(){
   destroyWorld();
}

void World::initialize(std::string filename){
    // Create object ifstream to input values
    std::ifstream input(filename);

    // If could not open then return error code
    if(!input){
        std::cerr << "Could not open file " << filename << "\n";
        throw 6;
    }

    WorldElement* worldElem = nullptr;
    int elements = 0;
    char type = '\0';
    input >> this->bandera >> std::ws;

    this->myWorld = std::vector<std::queue<WorldElement*>>(this->bandera);

    for(int slot = 0; slot < this->bandera; ++slot){
        input >> elements >> std::ws;
        for(int index = 0; index < elements; ++index){
            input >> type >> std::ws;
            if(type != 'N'){
                worldElem = createElement(type);
                if(!worldElem){
                    std::cerr << "Could not create worldElement\n";
                    throw 10;
                }
                this->myWorld[slot].push(worldElem);
            }
        }
    }

    // close file
    input.close();
}

std::queue<WorldElement *> World::getNext(std::string filename){
    if(this->position == bandera){
        destroyWorld();
        this->initialize(filename);
        this->position = 1;
    }
    return this->myWorld[this->position++];
}

#include "CoinElement.h"
#include "HoleElement.h"
#include "GoombaElement.h"
#include "KoopaTroopaElement.h"

WorldElement *World::createElement(const char element){
    if(element == 'C')
        return new CoinElement;
    else if(element == 'H')
        return new HoleElement;
    else if(element == 'G')
        return new GoombaElement;
    else if(element == 'K')
        return new KoopaTroopaElement;

    return nullptr;
}

void World::addEnemy(const char id){
    WorldElement* enemy = createElement(id);
    this->myWorld[(this->position + 10) % bandera].push(enemy);
}

void World::destroyWorld(){
    WorldElement* element = nullptr;
    for(std::queue<WorldElement*> myQueue : this->myWorld){
        while(!myQueue.empty()){
            element = myQueue.front();
            delete element;
            myQueue.pop();
        }
    }
}
