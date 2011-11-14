/* 
 * File:   SpaceItem.h
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on November 8, 2011, 9:40 AM
 */

#ifndef SPACEITEM_H
#define	SPACEITEM_H

#include "Board.h"
#include "Move.h"

/**
 * Predstavuje prvek prohledavaneho prostoru.
 * @author Tomáš Čerevka
 * @author Adam Činčura
 */
class SpaceItem {
public:

    /**
     * Konstruktor.
     */
    SpaceItem();

    /**
     * Konstruktor.
     * @param const Board& Aktualni deska.
     * @param const Move& Posledni tah.
     */
    SpaceItem(const Board&, const Move&);

    /**
     * Konstruktor.
     * @param const Board& Aktualni deska.
     * @param const vector<Move>& Tahy, ktere predchazely vytvoreni desky.
     */
    SpaceItem(const Board&, const vector<Move>&);

    /**
     * Konstruktor.
     * @param const Board& Aktualni deska.
     * @param const vector<Move>& Tahy, ktere predchazely vytvoreni desky.
     * @param const Move& Posledni tah.
     */
    SpaceItem(const Board&, const vector<Move>&, const Move&);


    /**
     * Destruktor.
     */
    virtual ~SpaceItem();

    /**
     * Vraci hloubku.
     * @return int Hloubka.
     */
    inline int getDepth(void) const {
        return moves.size() - 1;
        //return depth;
    }

    /**
     * Vraci tah.
     * @return const Move* Tah, kterym se deska dostala do tohoto stavu.
     */
    inline const Move* getMove(void) const {
        return &moves.back();
        //return &move;
    }

    inline const Board* getBoard(void) const {
        return &board;
    }

    inline const vector<Move>* getMoves(void) const {
        return &moves;
    }

    /**
     * Serializuje objekt.
     * @param char* Buffer.
     * @param int& Pozice v bufferu.
     */
    void serialize(char*, int&) const;

    /**
     * Deserializuje objekt.
     * @param char* Buffer.
     * @param int& Pozice v bufferu.
     */
    void deserialize(char*, int&);

    /**
     * Pretizeny operator pro vypis.
     * @param ostream& Vstupni stream.
     * @param SpaceItem& Vypisovany objekt.
     * @return ostream& Vystupni stream.
     */
    friend ostream& operator<<(ostream&, const SpaceItem&);

private:

    /** Aktualni stav desky. */
    Board board;

    /** Sekvence tahu, ktere vedou k aktualnimu stavu desky. */
    vector<Move> moves;
};

#endif	/* SPACEITEM_H */

