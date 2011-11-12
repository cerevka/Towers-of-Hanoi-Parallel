/* 
 * File:   SpaceItem.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 8, 2011, 9:40 AM
 */

#include "SpaceItem.h"

SpaceItem::SpaceItem(const Board& _board, const Move& _move, const int _depth) :
board(_board), move(_move), depth(_depth) {
    // Predane reference si do sebe zkopiruje.
}

SpaceItem::~SpaceItem() {
}

