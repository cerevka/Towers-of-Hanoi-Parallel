/* 
 * File:   SpaceItem.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 8, 2011, 9:40 AM
 */

#include "SpaceItem.h"

SpaceItem::SpaceItem() {
    
}

SpaceItem::SpaceItem(const Board& _board, const Move& _move) :
board(_board) {
    moves.push_back(_move);
}

SpaceItem::SpaceItem(const Board& _board, const vector<Move>& _moves, const Move& _move) :
board(_board), moves(_moves) {
    moves.push_back(_move);
}

SpaceItem::~SpaceItem() {
}

void SpaceItem::serialize(char* _buffer, int& _position) const {
    // Serializuje se deska.
    board.serialize(_buffer, _position);
    
    // Serializuje se historie tahu.
    int size = moves.size();
    MPI_Pack(&size, 1, MPI_INT, _buffer, BUFFER_SIZE, &_position, MPI_COMM_WORLD);
    
    // Serializuji se tahy v historii.
    for (vector<Move>::const_iterator it = moves.begin(); it < moves.end(); ++it) {
        it->serialize(_buffer, _position);
    }
}

void SpaceItem::deserialize(char* _buffer, int& _position) {
    // Deserializuje se deska.
    board.deserialize(_buffer, _position);
    
    // Deserializuje se historie tahu.
    int size;
    MPI_Unpack(_buffer, BUFFER_SIZE, &_position, &size, 1, MPI_INT, MPI_COMM_WORLD);
    for (int i = 0; i < size; ++i) {
        Move move;
        move.deserialize(_buffer, _position);
        moves.push_back(move);
    }
}

ostream& operator<<(ostream& _ostream, const SpaceItem& _spaceItem) {
    _ostream << _spaceItem.board << endl;
    _ostream << "History: " << endl;
    for (vector<Move>::const_iterator it = _spaceItem.moves.begin(); it < _spaceItem.moves.end(); ++it) {
        _ostream << *it << endl;
    }
    
    return _ostream;
}

