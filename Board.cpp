/* 
 * File:   Board.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 2, 2011, 10:19 AM
 */

#include "Board.h"
#include "Tower.h"

Board::Board(void) {

}

Board::Board(int _towersCount) : tokensCount(0) {
    init(_towersCount);
}

Board::~Board() {
}

void Board::init(int _towersCount) {
    if (_towersCount < 2) {
        throw "Minimal number of towers is two!";
    }

    for (int i = 0; i < _towersCount; ++i) {
        Tower tower;
        towers.push_back(tower); // Kopirovani.
    }

    tokensCount = 0;
}

bool Board::isMoveCorrect(const Move& _move) const {
    // Zdrojova vez je prazdna.
    if (towers[_move.getFrom()].isEmpty()) {
        return false;
    }

    // Cilova vez je prazda.
    if (towers[_move.getTo()].isEmpty()) {
        return true;
    }

    // Na obou vezich se nachazi token.
    // Na zdrojove vezi musi byt mensi nez na cilove.
    if (towers[_move.getFrom()].getTop() < towers[_move.getTo()].getTop()) {
        return true;
    }
    return false;

}

void Board::doMove(const Move& _move) {
    int token = towers[_move.getFrom()].removeTop();
    towers[_move.getTo()].addTop(token);
}

void Board::addTowerTop(int _tower, int _value) {
    towers[_tower].addTop(_value);
    ++tokensCount;
}

int Board::getTowerTop(int _tower) const {
    if (towers[_tower].size() == 0) {
        return 0;
    }
    return towers[_tower].getTop();
}

bool Board::isTowerComplete(int _tower) const {    
    if (towers[_tower].size() == tokensCount) {
        return true;
    }
    return false;
}

int Board::getLowerBound(int _tower) const {
    const Tower* const tower =  &(towers[_tower]);
    if (tower->size() == 0) return tokensCount;
    int ok = 0;
    for (int i = tokensCount; i > 0; --i) {
        if ( (tokensCount - i) >=  tower->size()) continue;
        if (tower->getToken(tokensCount - i) == i) ++ok;
    }
    return (tower->size() - ok) * 2 + tokensCount - tower->size();
}

ostream& operator<<(ostream& _ostream, const Board& _board) {   
    int order = 0;   
    for (vector<Tower>::const_iterator it = _board.towers.begin(); it != _board.towers.end(); ++it) {       
        _ostream << "Tower " << order++ << ": " << (*it) << endl;
    }
    return _ostream;
}

