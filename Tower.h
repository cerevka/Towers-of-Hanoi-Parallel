/* 
 * File:   Tower.h
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on November 2, 2011, 10:26 AM
 */

#ifndef TOWER_H
#define	TOWER_H

#include <vector>
#include <iostream>

using namespace std;

/**
 * Vez s tokeny.
 * @author Tomáš Čerevka
 * @author Adam Činčura
 */
class Tower {
public:    
    
    /**
     * Konstruktor.
     */
    Tower();   
    
    /**
     * Destruktor.
     */
    virtual ~Tower();
    
    /**
     * Prida token na vrchol vez.
     * @param int Hodnota tokenu.
     */
    void addToken(int);
    
    /**
     * Sunda token z vrcholu veze.
     * @return Hodnota sundaneho tokenu.
     */
    int removeToken(void);
    
    /**
     * Pretizeny operator vypisu do streamu.
     * @param ostream& Vstupni stream.
     * @param Tower& Vypisovana vez.
     * @return ostream& Vystupni stream.
     */
    friend ostream& operator<<(ostream&, const Tower&);    
    
private:
    
    /** Tokeny na vezi. */
    vector<int> tokens;

};

#endif	/* TOWER_H */

