/* 
 * File:   Solver.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 2, 2011, 10:31 AM
 */

#include "Solver.h"
#include "Board.h"
#include "mpi.h"

static const int MASTER_RANK = 0;
static const int TOKEN_WHITE = 10;
static const int TOKEN_BLACK = 11;
//kody jednotlivych zprav
static const int MESSAGE_TOKEN = 100;
static const int MESSAGE_FINISHED = 101;
static const int MESSAGE_WORK_REQUEST = 102;
static const int MESSAGE_WORK_NONE = 103;
static const int MESSAGE_WORK_DATA = 104;

enum ProcessColor {
    BLACK,
    WHITE
};

int tokenColor;
ProcessColor processColor; // zmena barvy se provede pri poslani prace...zatim neimplementovano
bool requestWork;
int myRank;
bool tokenArrived;
bool finished;
bool firstWorkDistribution;
int actualDepth = 1;
int dolniMez;

Solver::Solver(const Board* _board, int _targetTower, int _maxDepth) :
targetTower(_targetTower), maxDepth(_maxDepth), pushCount(0
) {
    initBoard = _board;
}

Solver::~Solver() {
    while (!space.empty()) {
        SpaceItem* spaceItem = space.back();
        delete spaceItem;
        spaceItem = NULL;
        space.pop_back();
    }
}

void Solver::init() {
    // na zacatku maji vsechny procesory bilou barvu
    processColor = WHITE;
    tokenColor = TOKEN_WHITE;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    requestWork = true;
    firstWorkDistribution= true;

    if (myRank == MASTER_RANK) {//master posle prvniho peska
        tokenArrived = true;
    } else {
        tokenArrived = false;
    }
}

void Solver::solve(vector<Move>& _solution) {
    lowerBound = initBoard->getLowerBound(targetTower);
    bestSolutionDepth = maxDepth;

    if (myRank != MASTER_RANK) {
        //cekam az mi prijde prace
        //prvni rozdeleni se udela automaticky bez zadani
        //cekam dokud message neprijde melo by prijit prvni deleni prace
        //momentalne to prvni spocte takzee prijde pesek
        MPI_Status s;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);

        processMessages();

    } else {
        // Hotovo, koncim.
        if (initBoard->isTowerComplete(targetTower)) {
            broadcast(MESSAGE_FINISHED);
            return;
        }
        Move firstMove(0, 0, 0);
        SpaceItem* firstSpaceItem = new SpaceItem(*initBoard, firstMove);
        space.push_back(firstSpaceItem);
        expandTop();
        actualDepth = 1;
    }
    int counter = 0;

    while (!finished) {
        if (!space.empty()) {
            proccessTop(_solution, actualDepth);
        } else {
            //pozadam o praci ---zatim neumim
            sendToken();
        }

        if (counter == 150) {//natipovat idealni honotu
            //sem prvni rozdeleni prace---jen MASTER
            counter = 0;
            processMessages();
        }

        counter++;
    }

    cout << "PushCount process " << myRank << ": " << pushCount << endl;
}

void Solver::sendToken() {

    processColor = WHITE;
    if (tokenArrived) {//prisel token od mastra takze muzu posilat dal svuj
        int p; //kolik je celkem procesu
        MPI_Comm_size(MPI_COMM_WORLD, &p);
        int dest = (myRank + 1) % p;

        MPI_Send(&tokenColor, 1, MPI_INT, dest, MESSAGE_TOKEN, MPI_COMM_WORLD);
        tokenArrived = false;
        cout << "Posilam peska pro: " << dest << endl;
    }
}

void Solver::broadcast(int message) {//Soch rikal ze nemame pouzivat metodu MPI_Bcast
    //tak jsem vytvoril tuhle mtodu, kera broadcast simuluje
    int p;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    for (int dest = 0; dest < p; dest++) {
        if (myRank != dest) {
            MPI_Send(&message, 1, MPI_INT, dest, MESSAGE_FINISHED, MPI_COMM_WORLD);
        }
    }
}

void Solver::processMessages() {
    int pesek; //sem si ulozim peska az mi prijde
    int flag;

    MPI_Status status, status2;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    while (flag) {
        switch (status.MPI_TAG) {//tady staci rozsirit o ostatni messages a rozumim vsemu
            case MESSAGE_WORK_NONE:{
                
                break;
            }
            
            case MESSAGE_WORK_DATA:{
                
                break;
            }
            
            
            
            case MESSAGE_TOKEN:
            {
                MPI_Recv(&pesek, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status2);
                cout << "Prisel Pesek od: " << status.MPI_SOURCE << endl;
                tokenArrived = true;

                if (pesek != TOKEN_BLACK && pesek != TOKEN_WHITE) {
                    cout << "Nespravna hodnoa peska" << endl;
                    throw "vadny pesek!";
                }
                if (processColor == WHITE) {
                    tokenColor = TOKEN_WHITE;
                } else {
                    tokenColor = TOKEN_BLACK;
                }

                //osetreni pokud jsem master
                if (myRank == MASTER_RANK) {

                    if (pesek == TOKEN_WHITE) {
                        cout << "Jsem Master a dorazil mi WHITE pesek...koncim vypocet" << endl;
                        broadcast(MESSAGE_FINISHED);
                        finished = true;
                        return;
                    }
                }
                break;
            }

            case MESSAGE_FINISHED:
            {
                finished = true;
                return;
            }
            default: {
                throw "Dorazila neplatna zprava!";
                break;
            }

        }
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
}

char Solver::proccessTop(vector<Move>& _solution, int& actualDepth) {
    // Je-li nalezene reseni rovne dolni mezi, nelze jiz najit lepsi.
    if (_solution.size() == lowerBound) {
        return 1;
    }

    // Jiz expandovany stav se odstrani pri navratu do nej.        
    if (actualDepth > space.back()->getDepth()) {
        SpaceItem* spaceItem = space.back();
        space.pop_back();
        delete spaceItem;
        spaceItem = NULL;
        actualSolution.pop_back();
        --actualDepth;
        return 0;
    }

    // Nasel jsem reseni.
    if (space.back()->getBoard()->isTowerComplete(targetTower)) {
        actualSolution.push_back(*(space.back()->getMove()));
        // Reseni je lepsi nez dosavadni.
        if (_solution.size() == 0 || _solution.size() > actualSolution.size()) {
            _solution = actualSolution;
            bestSolutionDepth = _solution.size();
        }
        actualSolution.pop_back();
        SpaceItem* spaceItem = space.back();
        space.pop_back();
        delete spaceItem;
        spaceItem = NULL;
        return 0;
    }

    // Jsem ve vetsi hloubce nez nejlepsi reseni.
    if (space.back()->getDepth() >= bestSolutionDepth) {
        SpaceItem* spaceItem = space.back();
        space.pop_back();
        delete spaceItem;
        spaceItem = NULL;
        return 0;
    }

    actualSolution.push_back(*(space.back()->getMove()));
    ++actualDepth;
    expandTop();
}

void Solver::expandTop(void) {
    int depth = space.back()->getDepth() + 1;
    const Board* actualBoard = space.back()->getBoard();
    const Move* lastMove = space.back()->getMove();
    const vector<Move>* history = space.back()->getMoves();
    for (int i = 0; i < actualBoard->size(); ++i) {
        for (int j = 0; j < actualBoard->size(); ++j) {
            // Delam-li tah na stejne vezi, koncim.
            if (i == j) continue;

            Move move(i, j, actualBoard->getTowerTop(i));

            if (actualBoard->isMoveCorrect(move) == false) continue;

            // Delam-li tah tam a zase zpatky, koncim.
            if (move.isReverse(*lastMove) == true) continue;

            // Zkopiruji si desku, protoze jsem ji doposud mel jen pro cteni.
            Board myBoard = *actualBoard;
            myBoard.doMove(move);
            int lowerBound = myBoard.getLowerBound(targetTower);

            // Pokud dalsi krok nemuze vest reseni z definice dolni meze, koncim.
            if ((depth + lowerBound) > bestSolutionDepth) continue;

            SpaceItem* spaceItem = new SpaceItem(myBoard, *history, move);
            space.push_back(spaceItem);
            ++pushCount;
        }
    }
}
