
#define JOCAN 0

#if JOCAN
#include "mpi.h"
#else
#include <mpi/mpi.h>
#endif

#include <climits>
#include <iostream>
#include <unistd.h>
#include "WorldElement.h"
#include "World.h"
#include "GameController.h"
#include "GameController.h"

void initArray(int *array, int size){
    for (int i = 0; i < size; ++i)
        array[i] = 0;
}

int GameController::attackRandom(Mario &myMario){
    int random = rand() % numProcesses;

    bool validEnemy = false;
    while(!validEnemy){
        if (this->mariosAlive[random] && random != (int)myMario.getRank()){
            validEnemy = true;
            continue;
        }
        random = rand() % numProcesses;
    }
    return random;
}

int GameController::leastCoins(Mario &myMario) const{
    int index = -1;
    int min = INT_MAX;
    for(int i = 0; i < numProcesses; ++i){
        if (this->mariosAlive[i] && i != (int)myMario.getRank())
            index = this->mariosCoins[i] < min?i:index;
    }
    return index;
}

int GameController::mostCoins(Mario &myMario) const{
    int index = -1;
    int max = INT_MIN;
    for(int i = 0; i < numProcesses; ++i){
        if (this->mariosAlive[i] && i != (int)myMario.getRank())
            index = this->mariosCoins[i] > max?i:index;
    }
    return index;
}

int GameController::attackingEnemy(Mario &myMario){
    int attackingEnemyVar = 0;

    for (int i = 0; i < numProcesses; ++i){
        if (this->mariosAlive[i] && this->attacking[i] == (int)myMario.getRank())
            attackingEnemyVar = i;
    }
    return attackingEnemyVar ? attackingEnemyVar:myMario.getEnemyRank();
}

void GameController::chooseEnemyRank(Mario &myMario){
    switch(myMario.getAttackStrategy()){
        case 'R':
        //Elije al azar
        myMario.setEnemyRank(attackRandom(myMario));
        break;
        case 'L':
        //Menos monedas
        myMario.setEnemyRank(leastCoins(myMario));
        break;
        case 'M':
        //Mas monedas
        myMario.setEnemyRank(mostCoins(myMario));
        break;
        case 'A':
        //Atacante
        myMario.setEnemyRank(attackingEnemy(myMario));
        break;
    }
}

GameController::GameController():clusterRun(false)
{
}

GameController::~GameController(){
    delete[] this->mariosAlive;
    delete[] this->mariosCoins;
    delete[] this->attacking;
    delete[] this->goombas;
    delete[] this->koopaTroopas;
}

int GameController::run(int argc, char** argv){
    int myRank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    srand(time( NULL ) * myRank);

    int marioToSee = -1;
    char strategy;

    this->initialize();

    World myWorld;
    try {
        myWorld.initialize("Worlds/Stage1-1.txt");       // Inicia el mundo
    } catch (int error) {
        return 2;
    }
    Mario myMario(myRank);
    struct timespec time;
    time.tv_nsec = 5, time.tv_sec = 0;

    try {
        validateInputData(myRank, argc, argv, marioToSee, strategy);    // Lee los argumentos de linea de comandos
    } catch (int error) {
        return 1;
    }

    MPI_Bcast(&strategy, 1, MPI_CHAR, 0, MPI_COMM_WORLD);       // le manda a todos la estrategia del Mario a ver
    MPI_Bcast(&marioToSee, 1, MPI_INT, 0, MPI_COMM_WORLD);      // Le manda a todos el mario

    if(myRank == marioToSee)
        myMario.setStrategy(strategy);

    int goombasSendBuffer[numProcesses];
    initArray(goombasSendBuffer, numProcesses);

    int koopasSendBuffer[numProcesses];
    initArray(koopasSendBuffer, numProcesses);

    int coins = myMario.getCoins();         // al principio todos tienen 0
    int alive = myMario.getIsAlive();       // al principio todos estan vivos, alive = 1
    int enemyRank = myMario.getEnemyRank();

    if(myRank == 0)
        myMario.killMario(), enemyRank = 0, coins = 0, alive = 0;

    GameController::synchronize(myMario, coins, alive, goombasSendBuffer, koopasSendBuffer, enemyRank, marioToSee);

    if(myRank){     // Si no soy el proceso 0
        //Se inicializa los arrays que contienen enemigos.
        initArray(this->goombas, numProcesses);
        initArray(this->koopaTroopas, numProcesses);

        while(myMario.getIsAlive() && checkBattleStatus()){
            //Aqui se atacan
            chooseEnemyRank(myMario);           
            addEnemies(myWorld, myMario);
            std::queue<WorldElement*> myQueue = myWorld.getNext("Worlds/Stage1-1.txt");

            if (myQueue.empty() && myRank == marioToSee)
                printMario(myWorld, myMario, " is walking");
            else
                advancePosition(myQueue, myMario, goombasSendBuffer, koopasSendBuffer, marioToSee, myWorld);

            if (!myMario.getIsAlive() && myRank == marioToSee)
                std::cout << "\033[0;31;47mMario #" << myRank << ". Game Over\033[0;00;00m\n";

            update(coins, alive, enemyRank, myMario, goombasSendBuffer, koopasSendBuffer, marioToSee);
            nanosleep(&time, NULL);
        }
    } else {
        while (checkBattleStatus()) //Mientras aun haya mas de 1 mario:
            GameController::synchronize(myMario, coins, alive, goombasSendBuffer, koopasSendBuffer, enemyRank, marioToSee);

        nanosleep(&time, NULL);
        std::cout << "\n\t\t|**\033[0;34;47mHAY UN GANADOR: es el Mario #"
                  << searchWinner() << " \\o/\033[0;00;00m**|\n\n";
    }

    // Para los muertos
    while(checkBattleStatus()){
        enemyRank = 0, coins = 0, alive = 0;
        initArray(koopasSendBuffer, numProcesses);
        initArray(goombasSendBuffer, numProcesses);
        GameController::synchronize(myMario, coins, alive, goombasSendBuffer, koopasSendBuffer, enemyRank, marioToSee);
    }

    MPI_Finalize();

    return 0;
}

void GameController::update(int& coins, int& alive, int& enemyRank, Mario& myMario, int* goombasSendBuffer, int* koopasSendBuffer, int& marioToSee ){
    coins = myMario.getCoins();
    alive = myMario.getIsAlive();
    enemyRank = myMario.getEnemyRank();
    initArray(this->goombas, numProcesses);
    initArray(this->koopaTroopas, numProcesses);
    GameController::synchronize(myMario, coins, alive, goombasSendBuffer, koopasSendBuffer, enemyRank, marioToSee);
    initArray(koopasSendBuffer, numProcesses);
    initArray(goombasSendBuffer, numProcesses);
}

void GameController::addEnemies(World& myWorld, Mario& myMario){
    while(this->goombas[myMario.getRank()]){
        myWorld.addEnemy('G');
        --this->goombas[myMario.getRank()];
    }

    while(this->koopaTroopas[myMario.getRank()]){
        myWorld.addEnemy('K');
        --this->koopaTroopas[myMario.getRank()];
    }
}


void GameController::synchronize(Mario &myMario, int &coins, int &alive, int* goombasSendBuffer, int* koopasSendBuffer, int &enemyRank, int &marioToSee){
    MPI_Allgather(&coins, 1, MPI_INT, this->mariosCoins, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(&alive, 1, MPI_INT, this->mariosAlive, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Allreduce(goombasSendBuffer, this->goombas, numProcesses, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(koopasSendBuffer, this->koopaTroopas, numProcesses, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allgather(&enemyRank, 1, MPI_INT, this->attacking, 1, MPI_INT, MPI_COMM_WORLD);

    if (myMario.getRank() == 0){
        if (!this->mariosAlive[marioToSee] && checkBattleStatus()){
            sleep(0);
            if (!this->clusterRun){
                do{
                    printBoard();
                    std::cout << "\033[0;34;47mSelect a new mario to spectate: \033[0;00;00m\n";
                    for (int i = 0; i < numProcesses; ++i){
                        if (this->mariosAlive[i])
                            std::cout << " " << i << " ";
                    }
                    std::cout << std::endl << "Option: ";
                    std::cin >> marioToSee;
                 } while(0 > marioToSee || marioToSee > numProcesses || !this->mariosAlive[marioToSee]);
            }else{
                for (int i = 0; i < numProcesses; ++i){
                    if (this->mariosAlive[i])
                        marioToSee = i;
                }
                printBoard();
                std::cout << "\033[0;34;47mCluster chose: "
                          << marioToSee << "\033[0;00;00m";
                printBoard();
            }
        }
    }
    MPI_Bcast(&marioToSee, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

void GameController::validateInputData(const int rank, const int argc, char** argv, int& marioToSee, char& strategy){
    char go = 'Y';
    if(rank == 0){
        // Validation of input data
        if(argc >= 3){
            marioToSee = atoi(argv[1]);
            if(marioToSee >= numProcesses || marioToSee <= 0)
                throwError(5, "Error: invalid mario chosen");

            strategy = *argv[2];
            if(!isValid(strategy))
                strategy = 'R';

            MPI_Bcast(&go, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
            if(argc > 3){
                std::string str = argv[3];
                if(str == "cluster")
                    this->clusterRun = true;
            }
        }else{
            throwError(1, "Error: Invalid number of parameters\nUsage: mpiexec -n numProcesses ./super_mario_mpi marioToSee strategy [cluster]");
        }
    }else
        MPI_Bcast(&go, 1, MPI_CHAR, 0, MPI_COMM_WORLD);     // 1, 2, 3

    if(go == 'N')
        throw 2; // THROW
}

void GameController::advancePosition(std::queue<WorldElement*>& myQueue, Mario& myMario, int* goombasSendBuffer, int* koopasSendBuffer, const size_t marioToSee, World& myWorld){
    WorldElement* element;
    std::vector<std::string> actions;

    while(!myQueue.empty() && myMario.getIsAlive()){
        element = myQueue.front();
        actions = element->getActions();
        int actionsSize = actions.size();
        std::vector<double> probabilities(actionsSize);
        double reference = rand()/(double)RAND_MAX;
        int index = 0;
        for(const std::string string : actions)
            probabilities[index++] = element->getActionProbability(string);

        double probabilyAccumluated = 0;
        int counter = 0;
        for(; counter <  actionsSize; ++counter){
            probabilyAccumluated += probabilities[counter];
            if (reference < probabilyAccumluated){
                element->doAction(myMario, actions[counter], element->getActionProbability("NO_JUMP") == 0.05? goombasSendBuffer:koopasSendBuffer);
                break;
            }
        }

        if (myMario.getRank() == marioToSee)
            printMario(myWorld, myMario, element->returnAction(actions[counter]));

        myQueue.pop();
    }
}

void GameController::printMario(World& myWorld, Mario& myMario, std::string stringAction){
    std::cout << "World pos. " << myWorld.getPosition() << ": "
              << "Mario #" << myMario.getRank() << stringAction << ". Coins: " << myMario.getCoins() << " | "
              << "atacking # " << myMario.getEnemyRank() << " | being attacked by";
    for (int i = 0; i < numProcesses; ++i){
        if (this->attacking[i] == (int)myMario.getRank())
            std::cout << " #" << i;
    }

    std::cout << " | attack strategy: " << myMario.getStrategyString() << " | Total playing: " << checkBattleStatus() << std::endl;
}

int GameController::searchWinner() const{
    for(int mario = 0; mario < this->numProcesses; ++mario){
        if(this->mariosAlive[mario])
            return mario;
    }
    return -1;
}

void GameController::throwError(const int error, const std::string strError)const{
    char go = 'N';
    MPI_Bcast(&go, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    std::cerr << strError << std::endl;
    throw error; // THROW
}

void GameController::initialize(){
    this->mariosAlive = new int[numProcesses]();
    this->mariosCoins = new int[numProcesses]();
    this->attacking = new int[numProcesses]();
    this->goombas = new int[numProcesses]();
    this->koopaTroopas = new int[numProcesses]();
}

int GameController::checkBattleStatus(){
    int sum = 0;
    for (int index = 0; index < this->numProcesses; ++index)
        sum += this->mariosAlive[index];
    if (sum >= 2)
        return sum;
    return 0;
}

