/* 
 * File:   Solver.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 2, 2011, 10:31 AM
 */

#include "Solver.h"
#include "Board.h"

Solver::Solver(const Board* _board, int _targetTower, int _maxDepth) :
targetTower(_targetTower), maxDepth(_maxDepth) {
    initBoard = _board;
}

Solver::~Solver() {

}

void Solver::solve(vector<Move>&) {
    throw "Not implemented yet.";
}

void Solver::expandStack(void) {
    throw "Not implemented yet.";    
}

