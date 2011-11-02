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

ostream& operator<<(ostream& _ostream, const Move& _move) {
    _ostream << "[" << _move.value << ", " << _move.from << " -> " << _move.to << "]";
    return _ostream;
}


