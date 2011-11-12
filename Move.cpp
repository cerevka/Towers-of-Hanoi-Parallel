/* 
 * File:   Move.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 2, 2011, 10:21 AM
 */

#include "Move.h"

Move::Move() {

}

Move::Move(int _from, int _to, int _value) :
from(_from), to(_to), value(_value) {

}

bool Move::isReverse(const Move& _move) const {
    return ( (this->from == _move.to) && (this->to == _move.from));
}

void Move::getReverse(const Move& _move, Move& _reverseMove) const {
    _reverseMove.from = _move.to;
    _reverseMove.to = _move.from;
    _reverseMove.value = _move.value;
}

void Move::serialize(char* _buffer, int& _position) const {
    int data = from;
    MPI_Pack(&data, 1, MPI_INT, _buffer, BUFFER_SIZE, &_position, MPI_COMM_WORLD);
    data = to;
    MPI_Pack(&data, 1, MPI_INT, _buffer, BUFFER_SIZE, &_position, MPI_COMM_WORLD);
    data = value;
    MPI_Pack(&data, 1, MPI_INT, _buffer, BUFFER_SIZE, &_position, MPI_COMM_WORLD);
}

void Move::deserialize(char* _buffer, int& _position) {
    MPI_Unpack(_buffer, BUFFER_SIZE, &_position, &from, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(_buffer, BUFFER_SIZE, &_position, &to, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(_buffer, BUFFER_SIZE, &_position, &value, 1, MPI_INT, MPI_COMM_WORLD);    
}

bool Move::operator==(const Move& _move) {
    return (from == _move.from && to == _move.to && value == _move.value);
}

ostream& operator<<(ostream& _ostream, const Move& _move) {
    _ostream << "[" << _move.value << ", " << _move.from << " -> " << _move.to << "]";
    return _ostream;
}


