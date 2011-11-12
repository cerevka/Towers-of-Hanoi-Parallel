/* 
 * File:   Move.h
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on November 2, 2011, 10:21 AM
 */

#ifndef MOVE_H
#define	MOVE_H

#include <iostream>
#include <mpi.h>

#include "global.h"

using namespace std;

/**
 * Tah ve hre - co, odkud a kam.
 * @author Tomáš Čerevka
 * @author Adam Činčura
 */
class Move {
public:

    /**
     * Konstruktor.
     */
    Move();

    /**
     * Konstruktor.
     * @param int Index veze, ze ktere byl token odebran.
     * @param int Index veze, na kterou byl token pridan.
     * @param int Hodnota presuneteho tokenu.
     */
    Move(int, int, int);

    /**
     * Vrati index veze, ze ktere byl token odebran.
     * @return int Index veze.
     */
    inline int getFrom(void) const {
        return this->from;
    }

    /**
     * Vrati index veze, na kterou byl token pridan.
     * @return int Index veze.
     */
    inline int getTo(void) const {
        return this->to;
    }

    /**
     * Vrati hodnotu presunuteho tokenu.
     * @return int Hodnota tokenu.
     */
    inline int getValue(void) const {
        return this->to;
    }

    /**
     * Rozhodne, zda jsou tahy inverzni.
     * @param const Move& Tah, se kterym porovnava.
     * @return TRUE jsou reverzni, jinak FALSE
     */
    bool isReverse(const Move&) const;

    /**
     * Vytvori reverzeni tah.
     * @param const Move& Tah, ke kteremu se bude konstruovat inverzni tah.
     * @param Move& Tah, do ktereho se inverzni tah zkonstruuje. 
     */
    void getReverse(const Move&, Move&) const;

    /**
     * Pretizeny operator pro porovnani dvou tahu.
     * @param Move& Tah, se kterym se porovnava.
     * @return TRUE jsou-li tahy stejne, jinak FALSE.
     */
    bool operator==(const Move&);

    /**
     * Serializuje tah.
     * @param char* Buffer.
     * @param int& Pozice v bufferu.
     */
    void serialize(char*, int&) const;

    /**
     * Deserialuzuje tah.
     * @param char* Buffer.
     * @param int& Pozice v bufferu.
     */
    void deserialize(char*, int&);

    /**
     * Pretizeny operator vypisu do streamu.
     * @param ostream& Vstupni stream.
     * @param const Move& Vypisovany tah.
     * @return ostream& Vystupni stream.
     */
    friend ostream& operator<<(ostream&, const Move&);


private:

    /** Index veze, ze ktere byl token odebran. */
    int from;

    /** Index veze, na kterou byl token pridan. */
    int to;

    /** Hodnota presunuteho tokenu, pouze pro kontrolu. */
    int value;

};

#endif	/* MOVE_H */

