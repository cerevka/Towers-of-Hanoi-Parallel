/* 
 * File:   Tower.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 2, 2011, 10:26 AM
 */

#include "Tower.h"

Tower::Tower() {
}

Tower::~Tower() {
}

void Tower::addTop(int token) {
    tokens.push_back(token);    
}

int Tower::getTop() const {
    return tokens.back();
}

int Tower::removeTop() {
    int token = getTop();
    tokens.pop_back();
    return token;
}

bool Tower::isEmpty(void) const {
    return tokens.empty();    
}

int Tower::size() const {
    return tokens.size();
}

ostream& operator<<(ostream& _ostream, const Tower& _tower) {
    string separator = "";
    for (vector<int>::const_iterator it = _tower.tokens.begin(); it < _tower.tokens.end(); ++it) {
        _ostream << separator << *it;
        separator = " ";
    }
    return _ostream;
}

