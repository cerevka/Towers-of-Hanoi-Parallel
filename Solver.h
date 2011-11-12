/* 
 * File:   Solver.h
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on November 2, 2011, 10:31 AM
 */

#ifndef SOLVER_H
#define	SOLVER_H

#include <vector>
#include "Move.h"
#include "Board.h"
#include "SpaceItem.h"

using namespace std;

/**
 * Resis zadani. Obsluhuje celou logiku algoritmu.
 * @author Tomáš Čerevka
 * @author Adam Činčura
 */
class Solver {
public:
    
    /**
     * Konstruktor.
     * @param const Board* Deska se vstupnimi daty.
     * @param int Index cilove veze.
     * @param int Maximalni hloubka prohledavaneho prostoru.
     */
    Solver(const Board*, int, int);

    /**
     * Destruktor. 
     */
    virtual ~Solver();
    
    /**
     * Hleda reseni nad vstupnimi daty.
     * @param vector<Move>& Vektor tahu, ktere vedou k reseni. Neexistuje-li 
     * reseni, je vektor prazdny.
     */
    void solve(vector<Move>&);

private:
    
    /** Index cilove veze. */
    int targetTower;
    
    /** Maximalni hloubka prohledavenho prostoru. */
    int maxDepth;
    
    /** Celkovy pocet prostoru vlozeny na zasobnik. */
    int pushCount;
    
    /** Vychozi deska. */
    const Board* initBoard;
    
    int bestSolutionDepth;
    
    /** Zasobnik prohledavanych prostoru. */
    vector<SpaceItem*> space;
    
    /**
     * Udela expanzi vrcholu zasobniku.
     */
    void expandTop(void);

};

#endif	/* SOLVER_H */

