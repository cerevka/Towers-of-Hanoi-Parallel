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

void Tower::serialize(char* _buffer, int& _position) const {
    // Zabali se informace o velikosti veze.
    int data = size();
    MPI_Pack(&data, 1, MPI_INT, _buffer, BUFFER_SIZE, &_position, MPI_COMM_WORLD);

    // Zabali se tokeny na vezi.
    for (vector<int>::const_iterator it = tokens.begin(); it < tokens.end(); ++it) {
        data = *it;
        MPI_Pack(&data, 1, MPI_INT, _buffer, BUFFER_SIZE, &_position, MPI_COMM_WORLD);
    }
}

void Tower::deserialize(char* _buffer, int& _position) {
    // Precte se velikost veze.
    int size;
    MPI_Unpack(_buffer, BUFFER_SIZE, &_position, &size, 1, MPI_INT, MPI_COMM_WORLD);

    // Prectou se tokeny na vezi.
    int token;
    for (int i = 0; i < size; ++i) {
        MPI_Unpack(_buffer, BUFFER_SIZE, &_position, &token, 1, MPI_INT, MPI_COMM_WORLD);
        addTop(token);
    }
}

ostream& operator<<(ostream& _ostream, const Tower& _tower) {
    string separator = "";
    for (vector<int>::const_iterator it = _tower.tokens.begin(); it < _tower.tokens.end(); ++it) {
        _ostream << separator << *it;
        separator = " ";
    }
    return _ostream;
}

