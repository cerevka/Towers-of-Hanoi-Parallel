/* 
 * File:   Tower.h
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on November 2, 2011, 10:26 AM
 */

#ifndef TOWER_H
#define	TOWER_H

#include <string>
#include <vector>
#include <iostream>
#include "Serializable.h"

using namespace std;

/**
 * Vez s tokeny.
 * @author Tomáš Čerevka
 * @author Adam Činčura
 */
class Tower : public Serializable {
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
    void addTop(int);
    
    /**
     * Vrati hodnotu tokenu na vrcholu veze.
     * @return int Hodnota tokenu na vrcholu veze.
     */
    int getTop(void) const;
    
    /**
     * Vrati hodnotu tokenu na dane pozici.
     * @param Index tokenu.
     * @return Hodnota vybraneho tokenu.
     */
    inline int getToken(int _token) const {
        return tokens[_token];
    }
    
    /**
     * Sunda token z vrcholu veze.
     * @return int Hodnota sundaneho tokenu.
     */
    int removeTop(void);
    
    /**
     * Rozhodne, zda je vez prazdna.
     * @return bool TRUE vez je prazdna, jinak FALSE.
     */
    bool isEmpty(void) const;
    
    /**
     * Vrati pocet tokenu na vezi.
     * @return int Pocet tokenu na vezi.
     */
    int size(void) const;
    
    /**
     * Pretizeny operator vypisu do streamu.
     * @param ostream& Vstupni stream.
     * @param Tower& Vypisovana vez.
     * @return ostream& Vystupni stream.
     */
    friend ostream& operator<<(ostream&, const Tower&); 
    
    /**
     * Serializuje vez.
     * @param char* Buffer.
     * @param int& Pozice v bufferu.
     */
    void serialize(char*, int&) const;
    
    /**
     * Deserializuje vez.
     * @param char* Buffer.
     * @param int& Pocize v bufferu.
     */
    void deserialize(char*, int&);
    
private:
    
    /** Tokeny na vezi. */
    vector<int> tokens;

};

#endif	/* TOWER_H */

