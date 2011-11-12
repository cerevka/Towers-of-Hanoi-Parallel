/* 
 * File:   SpaceItem.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 8, 2011, 9:40 AM
 */

#include "SpaceItem.h"

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

