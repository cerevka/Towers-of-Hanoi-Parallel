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
     * @param const Board& Aktualni deska.
     * @param const Move& Posledni tah, ktery desku uvedl do tohoto stavu.
     * @param int Hloubka prohledavaneho prostoru.
     */
    SpaceItem(const Board&, const Move&, int);
    
    
    /**
     * Destruktor.
     */
    virtual ~SpaceItem();
    
    /**
     * Vraci hloubku.
     * @return int Hloubka.
     */
    inline int getDepth(void) const {
        return depth;
    }
    
    /**
     * Vraci tah.
     * @return const Move* Tah, kterym se deska dostala do tohoto stavu.
     */
    inline const Move* getMove(void) const {
        return &move;
    }
    
    inline const Board* getBoard(void) const {
        return &board;
    }
    
private:
    
    /** Aktualni stav desky. */
    const Board board;
    
    /** Posledni tah, ktery desku uvedl do aktualniho stavu. */
    const Move move;
    
    /** Hloubka prohledavaneho prostoru. */
    const int depth;
};

#endif	/* SPACEITEM_H */

