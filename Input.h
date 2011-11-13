/* 
 * File:   Input.h
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on November 2, 2011, 10:28 AM
 */

#ifndef INPUT_H
#define	INPUT_H

#include <string>
#include <sstream>
#include <fstream>
#include "Board.h"

using namespace std;

/**
 * Obsluha zpracovani vstupnich dat.
 * @author Tomáš Čerevka
 * @author Adam Činčura
 */
class Input {
public:
    
    /**
     * Konstruktor.
     * @param Board* Deska, ktera se ma pripravit.
     */
    Input(Board*);
   
    /**
     * Destruktor.
     */
    virtual ~Input();
    
    /**
     * Vrati pocet vezi.
     * @return int Celkovy pocet vezi.
     */
    inline int getTowersCount(void) const {
        return towersCount;
    }
    
    /**
     * Vrati index cilove veze.
     * @return int Index cilove veze.
     */
    inline int getTargetTower(void) const {
        return targetTower;
    }
    
    /**
     * Vrati maximalni hloubku.
     * @return int Maximalni hloubka prohledavaneho prosturu.
     */
    inline int getMaxDepth(void) const {
        return maxDepth;
    }
    
    /**
     * Zpracuje vstupni argumenty.
     * @param int Pocet argumentu.
     * @param char** Pole argumentu.
     */
    void parseArguments(int, char**);
        
     
    /**
     * Vytiskne sumarizaci vstupu.
     */
    void printTask(void) const;
    
private:

    /** Vytvarena deska. */
    Board* board;
    
    /** Celkovy pocet vezi. */
    int towersCount;
    
    /** Pocet tokenu na vezich. */
    int tokensCount;
    
    /** Index cilove veze. */
    int targetTower;
    
    /** Maximalni hloubka. */
    int maxDepth;
    
    /** Nazev souboru se vstupnimi daty. */
    string fileName;
    
    /**
     * Zprarsuje vstupni soubor s nazvem {@link #filename}.
     */
    void parseFile(void);
    
    /**
     * Vygeneruje desku dle sveho vnitrniho stavu.
     */
    void generateBoard(void);
   
    
    /**
     * Vytiskne napovedu.
     */
    void printHelp(void) const;
    
};

#endif	/* INPUT_H */

