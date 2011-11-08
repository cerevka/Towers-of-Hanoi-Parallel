/* 
 * File:   Board.h
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on November 2, 2011, 10:19 AM
 */

#ifndef BOARD_H
#define	BOARD_H

#include <vector>

#include "Tower.h"
#include "Move.h"

using namespace std;

/**
 * Hraci deska, na ktere se nachazi jednotlive veze.
 * @author Tomáš Čerevka
 * @author Adam Činčura
 */
class Board {
public:
    
    /**
     * Konstrutor.
     * @param int Pocet vezi.
     */
    Board(int);
    
    /**
     * Destruktor.
     */
    virtual ~Board();
    
private:
    
    /** Mnozina vezi. */
    vector<Tower> towers;
    
    /** Celkovy pocet tokenu. */
    int tokensCount;
    
    /**
     * Rozhodne, zda je tah na desce mozny.
     * @param const Move& Posuzovany tah.
     * @return TRUE tah je mozny, jinak FALSE
     */
    bool isMoveCorrect(const Move&) const;
    
    /**
     * Vykona nad deskou tah.
     * @param const Move& Provadeny tah.
     */
    void doMove(const Move&);
    
    /**
     * Prida token na vrchol veze.
     * Nekontroluje, zda pak bude vez validni!
     * @param int Index veze.
     * @param int Hodnota tokenu.
     */
    void addTowerTop(int, int);
    
    /**
     * Rozhodne, zda je vez kompletni.
     * @param int Index veze.
     * @return TRUE vez je kompletni, jinak FALSE.
     */
    bool isTowerComplete(int) const;
    
    /**
     * Vraci dolni mez reseni - minimalni pocet tahu, ktery je schopny
     * transformovat desku to hledaneho reseni.
     * @param int Index cilove veze.
     * @return int Pocet tahu potrebnych k transformaci.
     */
    int getLowBound(int) const;
    
    /**
     * Pretizeni operatoru pro vypis do streamu.
     * @param ostream& Vstupni stream.
     * @param const Board& Vypisovany objekt.
     * @return ostream& Vystupni stream.
     */
    friend ostream& operator<<(ostream&, const Board&);

};

#endif	/* BOARD_H */

