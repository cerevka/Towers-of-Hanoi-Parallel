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
        SpaceItem* spaceItem = space.back();
        delete spaceItem;
        spaceItem = NULL;
        space.pop_back();
    }
}

void Solver::solve(vector<Move>& _solution) {
    lowerBound = initBoard->getLowerBound(targetTower);    
    bestSolutionDepth = maxDepth;

    // Hotovo, koncim.
    if (initBoard->isTowerComplete(targetTower)) return;

    Move firstMove(0, 0, 0);
    SpaceItem* firstSpaceItem = new SpaceItem(*initBoard, firstMove);
    space.push_back(firstSpaceItem);
    expandTop();
    int actualDepth = 1;

    while (!space.empty()) {
        int result = proccessTop(_solution, actualDepth);
        if (result == 1) {
            break;
        }
    }
    cout << "PushCount: " << pushCount << endl;
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
