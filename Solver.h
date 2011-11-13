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

     /**
     * Inicializuje vnitrni promenne solveru.
     */
    void init(void);
    
private:
    
    /** Index cilove veze. */
    int targetTower;
    
    /** Maximalni hloubka prohledavenho prostoru. */
    int maxDepth;
    
    /** Celkovy pocet prostoru vlozeny na zasobnik. */
    int pushCount;
    
    /** Vychozi deska. */
    const Board* initBoard;
    
    /** Hloubka nejlepsiho reseni. */
    int bestSolutionDepth;
    
    /** Zasobnik prohledavanych prostoru. */
    vector<SpaceItem*> space;
    
    /** Aktualni nalezene reseni. */
    vector<Move> actualSolution;
    
    /** Dolni mez reseni. */
    int lowerBound;
    
    /**
     * Zpracuje vrchol zasobniku.
     * @param vector<Move>& Nejlepsi reseni.
     * @param int& Aktualni hloubka.
     * @return 1 = prerusit cyklus, 0 = pokracovat
     */
    char proccessTop(vector<Move>&, int&);
    
    /**
     * Udela expanzi vrcholu zasobniku.
     */
    void expandTop(void);
    
   
    
    /**
     * Odesle token pro distribuovane ukonceni vypoctu.
     */
    void sendToken(void);
    
    
    /**
     * Odesle direktivni message vsem ostatnim procesum.
     * @param tag odesilane zpravy
     */
    void broadcast(int);
    
    /**
     * Zpracuje pripadne prichozi zpravy.
     */
    void processMessages(void);
};

#endif	/* SOLVER_H */

