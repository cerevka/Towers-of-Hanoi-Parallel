/* 
 * File:   Solver.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 2, 2011, 10:31 AM
 */

#include "Solver.h"
#include "Board.h"

Solver::Solver(const Board* _board, int _targetTower, int _maxDepth) :
targetTower(_targetTower), maxDepth(_maxDepth), pushCount(0
) {
    initBoard = _board;
}

Solver::~Solver() {
    while (!space.empty()) {
        SpaceItem* spaceItem = space.top();
        delete spaceItem;
        spaceItem = NULL;
        space.pop();
    }
}

void Solver::solve(vector<Move>& _solution) {
    int lowerBound = initBoard->getLowerBound(targetTower);    
    vector<Move> actualSolution;
    bestSolutionDepth = maxDepth;
    
    // Hotovo, koncim.
    if (initBoard->isTowerComplete(targetTower)) return;
    
    Move firstMove(0, 0, 0);
    SpaceItem* firstSpaceItem = new SpaceItem(*initBoard, firstMove);
    space.push(firstSpaceItem);
    expandTop();
    int actualDepth = 1;    
    
    while (!space.empty()) {       
        // Je-li nalezene reseni rovne dolni mezi, nelze jiz najit lepsi.
        if (_solution.size() == lowerBound) {           
            break;
        }
        
        // Jiz expandovany stav se odstrani pri navratu do nej.        
        if (actualDepth > space.top()->getDepth()) {            
            SpaceItem* spaceItem = space.top();
            space.pop();            
            delete spaceItem;
            actualSolution.pop_back();            
            --actualDepth;
            continue;
        }
        
        // Nasel jsem reseni.
        if (space.top()->getBoard()->isTowerComplete(targetTower)) {               
            actualSolution.push_back(*(space.top()->getMove()));
            // Reseni je lepsi nez dosavadni.
            if (_solution.size() == 0 || _solution.size() > actualSolution.size()) {               
                _solution = actualSolution;         
                bestSolutionDepth = _solution.size();                
            }
            actualSolution.pop_back();
            SpaceItem* spaceItem = space.top();
            space.pop();           
            delete spaceItem;
            continue;
        }
        
        // Jsem ve vetsi hloubce nez nejlepsi reseni.
        if (space.top()->getDepth() >= bestSolutionDepth) { 
            SpaceItem* spaceItem = space.top();
            space.pop();           
            delete spaceItem;
            continue;
        }              
        
        actualSolution.push_back(*(space.top()->getMove()));        
        ++actualDepth;        
        expandTop();          
    }    
    cout << "PushCount: " << pushCount << endl;
}

void Solver::expandTop(void) {     
    int depth = space.top()->getDepth() + 1;
    const Board* actualBoard = space.top()->getBoard();
    const Move* lastMove = space.top()->getMove();
    const vector<Move>* history = space.top()->getMoves(); 
    
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
            space.push(spaceItem);
            ++pushCount;            
        }
    }    
}
