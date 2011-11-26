/* 
 * File:   Solver.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 2, 2011, 10:31 AM
 */

#include "Solver.h"
#include "Board.h"
#include "mpi.h"
#include "WorkMessage.h"
#include <stdlib.h>

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
int numberOfProcesses;
int workRequestsInt=0;
int failedWorkRequests=0;
MPI_Request lastRequest;
MPI_Status status;
vector<int> *workRequests = new vector<int>();

void serializeSolution(char* buffer, int& position, vector<Move>& _solution) {
    int data = _solution.size();
    MPI_Pack(&data, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
    for (vector<Move>::const_iterator it = _solution.begin(); it < _solution.end(); ++it) {
        it->serialize(buffer, position);
    }
}

void deserializeSolution(char* buffer, int& position, vector<Move>& _solution) {
    int size;
    MPI_Unpack(buffer, BUFFER_SIZE, &position, &size, 1, MPI_INT, MPI_COMM_WORLD);
    for (int i = 0; i < size; ++i) {
        Move move;
        move.deserialize(buffer, position);
        _solution.push_back(move);
    }
}

void compare(char* input, char* inoutput, int* length, MPI_Datatype* type) {
    vector<Move> solution1;
    int position1 = 0;
    deserializeSolution(input, position1, solution1);

    vector<Move> solution2;
    int position2 = 0;
    deserializeSolution(inoutput, position2, solution2);

    int position3 = 0;
    if (solution1.size() ==0) {
        serializeSolution(inoutput, position3, solution2);
        return;
    } 
    if(solution2.size()==0){
         serializeSolution(inoutput, position3, solution1);
         return;
    }
    if(solution2.size()>solution1.size()){
         serializeSolution(inoutput, position3, solution1);
    }
    else{
         serializeSolution(inoutput, position3, solution2);
    }
}

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
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    requestWork = true;
    firstWorkDistribution = false;

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
        firstWorkDistribution = true;
        space.erase(space.begin());
    }
    int counter = 0;

    while (!finished) {
        if (!space.empty()) {
            proccessTop(_solution, actualDepth);
        } else {

            requestData();
            sendToken();
        }

        if (counter == 150) {//natipovat idealni honotu
            if ((myRank == MASTER_RANK) && firstWorkDistribution) {
                firstDistribution();
                counter = 0;
            } else {
                counter = 0;
                processMessages();

                if (finished) {
                    char buffer[BUFFER_SIZE];
                    int position = 0;

                    serializeSolution(buffer, position, _solution);

                    char result[BUFFER_SIZE];

                    MPI_Op op;
                    MPI_Op_create((MPI_User_function*) compare, 0, &op);
                  //  cout<<_solution.size()<<endl;
                    MPI_Reduce(&buffer, &result, BUFFER_SIZE, MPI_PACKED, op, 0, MPI_COMM_WORLD);
                    int workRequestsRes;
                    MPI_Reduce (&workRequestsInt, &workRequestsRes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
                    int failedWorkRequestsRes;
                    MPI_Reduce (&failedWorkRequests, &failedWorkRequestsRes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
                    if (myRank == MASTER_RANK) {
                        vector<Move> solution;
                        int position = 0;
                        deserializeSolution(result, position, solution);

                        cout << "MASTER - ZACATEK RESENI" << endl;
                        for (vector<Move>::const_iterator it = solution.begin(); it < solution.end(); ++it) {
                            cout << *it << endl;
                        }
                        cout << "Solution depth: " << solution.size() << endl;
                        cout<< "Work Requests: "<<workRequestsRes<<endl;
                        cout<<"Denied Work Requests: "<<failedWorkRequestsRes<<endl;
                        
                        cout << "MASTER - KONEC RESENI" << endl;
                    }
                }
            }
        }
        counter++;
    }

    //   cout << "PushCount process " << myRank << ": " << pushCount << endl;
    //   cout << "Solution: " << _solution.size() << " - process " << myRank << endl;
    //   cout << "mam jeste: " << space.size() << " stavu" << endl;
}

void Solver::requestData() {
    if (requestWork) {
        requestWork = false;
        ++workRequestsInt;
        int dest = rand() % numberOfProcesses; //nahodne vygeneruju koho pozadam o praci

        while (dest == myRank) {//neposilam zadost sam sobe
            dest = rand() % numberOfProcesses;
        }
        //  cout << "jsem: " << myRank << " chci data od: " << dest << endl;
        int foo = 0;
        MPI_Send(&foo, 1, MPI_INT, dest, MESSAGE_WORK_REQUEST, MPI_COMM_WORLD);

    }
}

void Solver::sendWork(int i) {//posilani v teto zprave mozna nahradit neblokujicim send

    // if (!workRequests->empty()) {
    if (!space.empty()) {
        if ((space.front()->getDepth()>(maxDepth - 9))||(space.size()<=2) ) {
            //   for (vector<int>::iterator it = workRequests->begin(); it < workRequests->end(); ++it) {
            //    cout << "NEposilam procesu: " << i << endl;
            int foo = 3;


            MPI_Isend(&foo, 1, MPI_INT, i, MESSAGE_WORK_NONE, MPI_COMM_WORLD, &lastRequest);
            // MPI_Wait(&lastRequest,&status);
        } else {
            if (myRank > i) {
                //     cout << "ZCERNAL SEM" << endl;
                processColor = BLACK;
            }
            WorkMessage m;

            vector<SpaceItem*>::iterator it = space.begin();
            it++;
            m.addItem(**it);
            char buffer[BUFFER_SIZE];
            int position = 0;
            m.serialize(buffer, position);

            MPI_Isend(buffer, position, MPI_PACKED, i, MESSAGE_WORK_DATA, MPI_COMM_WORLD, &lastRequest);
            //  SpaceItem* spaceItem = *it;
            // MPI_Wait(&lastRequest,&status);
            //  cout << "Pred: " << space.size();
            space.erase(it);
            //  cout << "Po: " << space.size() << endl;
        }
    } else {
        //   cout << "NEposilam procesu: " << i << endl;
        int foo = 3;


        MPI_Isend(&foo, 1, MPI_INT, i, MESSAGE_WORK_NONE, MPI_COMM_WORLD, &lastRequest);
        //  MPI_Wait(&lastRequest,&status);
    }
    //  workRequests->clear();
    //  return;
    //   }

    ///NAIVNI IMPLEMENTACE-KAZDYMU POSLE JEDEN STAV POPORADE
    //odmitnu ty co nemuzu naplnit
    /*while (workRequests->size() >= (space.size() - 1)) {
        int foo = 3;
            
        MPI_Isend(&foo, 1, MPI_INT, workRequests->back(), MESSAGE_WORK_NONE, MPI_COMM_WORLD, &lastRequest);
         MPI_Wait(&lastRequest,&status);
        workRequests->pop_back();
    }*/


    // delete spaceItem;
    //  spaceItem = NULL;

    //             if(minDepth>=(maxDepth-5)){//takhle hluboky stavy neposilam je to zbytecny
    //                  for(vector<int>::iterator it = workRequests->begin();it<workRequests->end();++it){
    //                cout<<"NEposilam procesu: "<<*it<<endl;
    //                int foo = 3;
    //                
    //                    MPI_Isend(&foo,1,MPI_INT,*it,MESSAGE_WORK_NONE,MPI_COMM_WORLD,&request);
    //            }
    //            workRequests->clear();
    //            return;
    //             }
    //            int requests = workRequests->size();
    //            int itemsOnLevel = 0;
    //            vector<SpaceItem*>::iterator it = space.begin();
    //
    //            while ((*it)->getDepth() <= minDepth) {
    //                if (minDepth == (*it)->getDepth()) ++itemsOnLevel;
    //                it++;
    //            }
    //           
    //            if(itemsOnLevel<=1){
    //                minDepth++;
    //                continue;
    //            }
    //             vector<SpaceItem*>::iterator iter = space.begin();
    //                //nalistuju zacatek aktualne zpracovavane urovne
    //                while ((*iter)->getDepth() < minDepth) {
    //                    iter++;
    //                }
    //            
    //            //mam mene stavu nez zadosti
    //            //kazdymu dam jeden stav a posledni si necham
    //            if (itemsOnLevel < (requests + 1)) {
    //                cout<<"Vetev jedna"<<endl;
    //                for (int i = 0; i < (itemsOnLevel - 1); ++i) {
    //                    WorkMessage m;
    //                    m.addItem(**iter);
    //                    char buffer[BUFFER_SIZE];
    //                    int position = 0;
    //                    m.serialize(buffer, position);
    //                    MPI_Send(buffer, position, MPI_PACKED, workRequests->front(), MESSAGE_WORK_DATA, MPI_COMM_WORLD);
    //                   
    //                    //barveni tokenu
    //                    if(myRank>workRequests->front()){
    //                        cout<<"ZCERNAL SEM"<<endl;
    //                         processColor = BLACK;
    //                        //  tokenColor=TOKEN_BLACK;
    //
    //                    }
    //                    //samzu poslany prvek a dostanu iterator na dalsi
    //                    SpaceItem * item=*iter;
    //                    iter = space.erase(iter);
    //                    delete item;
    //                    //smazu vyrizenou zadost
    //                    workRequests->erase(workRequests->begin());
    //                    
    //                }
    //            }                
    //            //mam vic stavu nez zadosti
    //            //budu posilat tolik stavu abych to o nejlip podelil
    //            else {
    //             //   cout<<"vetev dva"<<endl;
    //                 for (int i = 0; i < (requests); ++i) {
    //                    WorkMessage m;
    //                    m.addItem(**iter);
    //                    char buffer[BUFFER_SIZE];
    //                    int position = 0;
    //                    m.serialize(buffer, position);
    //                    MPI_Send(buffer, position, MPI_PACKED, workRequests->front(), MESSAGE_WORK_DATA, MPI_COMM_WORLD);
    //                  
    //                     if(myRank>workRequests->front()){
    //                         cout<<"ZCERNAL SEM"<<endl;
    //                       //  tokenColor=TOKEN_BLACK;
    //                         processColor = BLACK;
    //
    //                    }
    //                    //samzu poslany prvek a dostanu iterator na dalsi
    //                    SpaceItem * item=*iter;
    //                    iter = space.erase(iter);
    //                    delete item;
    //                    //smazu vyrizenou zadost
    //                    workRequests->erase(workRequests->begin());
    //                    
    //                    //--------------------------------------
    //                //kolik itemu bude v jedne zprave
    ////                int pocetItemu = (int) itemsOnLevel / (requests+1);
    ////                
    ////                while(!workRequests->empty()) {
    ////                    WorkMessage m;
    ////                    for(int i =0; i<pocetItemu;++i){
    ////                         m.addItem(**iter);
    ////                        }                   
    ////                    char buffer[BUFFER_SIZE];
    ////                    int position = 0;
    ////                    m.serialize(buffer, position);
    ////                    MPI_Send(buffer, position, MPI_PACKED, workRequests->front(), MESSAGE_WORK_DATA, MPI_COMM_WORLD);
    ////                   
    ////                    iter-=pocetItemu;
    ////                    cout<<"chystam se smazat:"<<**iter<<endl;
    ////                    for(int i = 0; i<pocetItemu;++i){
    ////                         SpaceItem * item=*iter;
    ////                   iter = space.erase(iter);
    ////                   delete item;
    ////                    }
    //               //     workRequests->erase(workRequests->begin());//smazu vyrozenou zadost
    //     }

    // minDepth++;//postupuju na dalsi patro            
    //}
    //  }
}

void Solver::firstDistribution() {
    // cout << "Prvni distribuce" << endl;

    //nasimuluju ze vsichni krome mastra chteji praci
    for (int i = 1; i < numberOfProcesses; ++i) {
        sendWork(i);
    }

    firstWorkDistribution = false;
}

void Solver::sendToken() {

    processColor = WHITE;
    if (tokenArrived) {//prisel token od mastra takze muzu posilat dal svuj
        int p; //kolik je celkem procesu
        MPI_Comm_size(MPI_COMM_WORLD, &p);
        int dest = (myRank + 1) % p;

        MPI_Send(&tokenColor, 1, MPI_INT, dest, MESSAGE_TOKEN, MPI_COMM_WORLD);
        tokenArrived = false;
        //  cout << "Posilam peska pro: " << dest << "barva: " << tokenColor << endl;
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
    int foo; //sem ukladam vystup ze zpravy co neposila data
    int flag;
    char buffer[BUFFER_SIZE];
    MPI_Status status, status2;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    while (flag) {
        switch (status.MPI_TAG) {//tady staci rozsirit o ostatni messages a rozumim vsemu
            case MESSAGE_WORK_NONE:
            {
                MPI_Recv(&foo, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status2);
                requestWork = true; //nastavim ze budu zadat znovu
                //  cout << "proces: " << status.MPI_SOURCE << "mi NEposlal praci." << endl;
                ++failedWorkRequests;
                break;
            }

            case MESSAGE_WORK_DATA:
            {
                MPI_Recv(buffer, BUFFER_SIZE, MPI_PACKED, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status2);
                int position = 0;
                WorkMessage message;
                message.deserialize(buffer, position);
                //   cout << "jsem: " << myRank << " Obdrzel jsem: " << message << endl;
               // actualDepth = message.getItems()->front().getDepth(); //nastavim spravnou aktualni hloubku
                //    cout << "jsem: " << myRank << "nastavuji aktualni hloubku na: " << actualDepth << endl;
                for (int i = 0; i < message.getItems()->size(); ++i) {//tady to muze skripat
                    SpaceItem* spaceItem = new SpaceItem(*(message.getItems()->at(i).getBoard()), *(message.getItems()->at(i).getMoves()));
                    space.push_back(spaceItem);
                    actualDepth=spaceItem->getDepth();
                }
                requestWork = true;
                break;
            }
            case MESSAGE_WORK_REQUEST:
            {
                // workRequests->push_back(status.MPI_SOURCE); //ulozim si kdo chce praci
                MPI_Recv(&foo, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status2);
                sendWork(status.MPI_SOURCE);
                break;
            }


            case MESSAGE_TOKEN:
            {
                MPI_Recv(&pesek, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status2);
                //    cout << "Prisel Pesek od: " << status.MPI_SOURCE << " a mam: " << space.size() << " prace" << endl;
                tokenArrived = true;

                if (pesek != TOKEN_BLACK && pesek != TOKEN_WHITE) {
                    //     cout << "Nespravna hodnoa peska" << endl;
                    throw "vadny pesek!";
                }
                if (processColor == BLACK) {
                    tokenColor = TOKEN_BLACK;
                } else {
                    tokenColor = TOKEN_WHITE;
                }

                //osetreni pokud jsem master
                if (myRank == MASTER_RANK) {

                    if (pesek == TOKEN_WHITE) {
                        //  cout << "Jsem Master a dorazil mi WHITE pesek...koncim vypocet" << endl;
                        broadcast(MESSAGE_FINISHED);
                        MPI_Barrier(MPI_COMM_WORLD);
                        finished = true;

                        return;
                    } else {
                        tokenColor = TOKEN_WHITE;
                    }
                }
                break;
            }

            case MESSAGE_FINISHED:
            {
                MPI_Barrier(MPI_COMM_WORLD);
                finished = true;

                return;
            }
            default:
            {
                throw "Dorazila neplatna zprava!";
                break;
            }

        }
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
    //sendWork();
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

            //if (lastMove->getTo()==move.getFrom()) continue;
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
