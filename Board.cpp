/* 
 * File:   Board.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 2, 2011, 10:19 AM
 */

#include "Board.h"
#include "Tower.h"

Board::Board(int _towersCount) : tokensCount(0) {
    if (_towersCount < 2) {
        throw "Minimal number of towers is two!";
    }
    
    for (int i = 0; i < _towersCount; ++i) {
        Tower tower;
        towers.push_back(tower); // Kopirovani.
    }
}


Board::~Board() {
}

ostream& operator<<(ostream& _ostream, const Board& _board) {
    // @TODO Vypis desky do streamu.
    return _ostream;
}

