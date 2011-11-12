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
    const Tower * const tower = &(towers[_tower]);
    if (tower->size() == 0) return tokensCount;
    int ok = 0;
    for (int i = tokensCount; i > 0; --i) {
        if ((tokensCount - i) >= tower->size()) continue;
        if (tower->getToken(tokensCount - i) == i) ++ok;
    }
    return (tower->size() - ok) * 2 + tokensCount - tower->size();
}

void Board::serialize(char* _buffer, int& _position) const {
    // Zabali se pocet tokenu na desce.
    int data = tokensCount;
    MPI_Pack(&data, 1, MPI_INT, _buffer, BUFFER_SIZE, &_position, MPI_COMM_WORLD);

    // Zabali se velikost desky (pocet vezi).
    data = size();
    MPI_Pack(&data, 1, MPI_INT, _buffer, BUFFER_SIZE, &_position, MPI_COMM_WORLD);

    // Zabali se vsechny veze na desce.
    for (vector<Tower>::const_iterator it = towers.begin(); it < towers.end(); ++it) {
        it->serialize(_buffer, _position);
    }
}

void Board::deserialize(char* _buffer, int& _position) {
    // Precte se pocet tokenu.
    MPI_Unpack(_buffer, BUFFER_SIZE, &_position, &tokensCount, 1, MPI_INT, MPI_COMM_WORLD);

    // Precte se velikost desky.
    int size;
    MPI_Unpack(_buffer, BUFFER_SIZE, &_position, &size, 1, MPI_INT, MPI_COMM_WORLD);

    // Deserializuji se veze.
    for (int i = 0; i < size; ++i) {
        Tower tower;
        tower.deserialize(_buffer, _position);
        towers.push_back(tower); // Kopie.
    }
}

ostream& operator<<(ostream& _ostream, const Board& _board) {
    int order = 0;
    for (vector<Tower>::const_iterator it = _board.towers.begin(); it != _board.towers.end(); ++it) {
        _ostream << "Tower " << order++ << ": " << (*it) << endl;
    }
    return _ostream;
}

