/* 
 * File:   WorkMessage.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 13, 2011, 11:32 AM
 */

#include "WorkMessage.h"

WorkMessage::WorkMessage() {
}


WorkMessage::~WorkMessage() {
}

void WorkMessage::addItem(SpaceItem& _spaceItem) {
    items.push_back(_spaceItem);
}

void WorkMessage::serialize(char* _buffer, int& _position) const {
    // Serializuje se pocet prvku.
    int size = items.size();
    MPI_Pack(&size, 1, MPI_INT, _buffer, BUFFER_SIZE, &_position, MPI_COMM_WORLD);
    
    // Serializuje se jednotlive prvky.
    for (vector<SpaceItem>::const_iterator it = items.begin(); it < items.end(); ++it) {
        it->serialize(_buffer, _position);
    }
}

void WorkMessage::deserialize(char* _buffer, int& _position) {
    // Deserializuje se pocet prvku.
    int size;
    MPI_Unpack(_buffer, BUFFER_SIZE, &_position, &size, 1, MPI_INT, MPI_COMM_WORLD);
    
    // Deserializuji se jednotlive prvky.
    for (int i = 0; i < size; ++i) {
        SpaceItem spaceItem;
        spaceItem.deserialize(_buffer, _position);
        items.push_back(spaceItem);
    }
}

ostream& operator<<(ostream& _ostream, const WorkMessage& _message) {
    int order = 1;
    _ostream << "CONTAINER" << endl;
    for (vector<SpaceItem>::const_iterator it = _message.items.begin(); it < _message.items.end(); ++it) {
        _ostream << order++ << ". item:" << endl;
        _ostream << *it << endl;
    }
    return _ostream;
}